#ifdef HAVE_BOOST_MULTI_INDEX_CONTAINER_HPP
#include "container.hpp"

#include "ruby_util.hpp"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

namespace rb_lovely { namespace hybrid {

struct member {
  bool operator<(member const& rhs) const {
    auto cmpResult = compareProc != Qnil ?
      rb_funcall(compareProc, callSym, 2, val, rhs.val) :
      rb_funcall(val, cmpMethSym, 1, rhs.val);
    return NUM2INT(cmpResult) < 0;
  }

  bool operator==(member const& rhs) const {
    auto equalityVal = rb_funcall(key, hashEqualitySym, 1, rhs.key);
    return RTEST(equalityVal);
  }

  // also cache as two element array?
  member(VALUE _compareProc, VALUE _key, VALUE _val)
    : compareProc(_compareProc), key(_key), val(_val) {}

  VALUE compareProc;
  VALUE key;
  VALUE val;
};

std::size_t hash_value(member const& member) {
  // TODO: something better?
  return reinterpret_cast<std::size_t>(&member);
  // return NUM2INT(rb_funcall(member.val, hashSym, 0));
}

namespace mi = boost::multi_index;

struct Hash {
  boost::multi_index_container<
    member,
    mi::indexed_by<
      mi::hashed_unique< mi::member<member, VALUE, &member::key> >,
      mi::ordered_non_unique< mi::identity<member> >
    >
  > container;

  // proc used to compare values
  VALUE compareProc = Qnil;
};

VALUE hashLength(VALUE self) {
  Hash* hash = rubyCast<Hash>(self);
  return INT2NUM(hash->container.size());
}

VALUE hashInitialize(int argc, VALUE *argv, VALUE self) {
  Hash* hash = rubyCast<Hash>(self);
  if (argc == 1) {
    auto array = rb_check_array_type(argv[0]);
    if (array == Qnil) {
      auto kwArgs = rb_check_hash_type(argv[0]);
      if (kwArgs == Qnil) {
        rb_raise(rb_eArgError, "Expected array");
      }
      else {
        auto compareProc = rb_hash_aref(kwArgs, compareSym);
        if (compareProc == Qnil) {
          rb_raise(rb_eArgError, "Expected compare keyword");
        }
        else {
          hash->compareProc = compareProc;
        }
      }
    }
    else {
      auto len = RARRAY_LEN(array);
      if (len % 2 != 0) {
        rb_raise(rb_eArgError, "Expected even number of parameters");
      }
      else {
        for (auto i = 0; i < len; i += 2) {
          hash->container.insert(member(hash->compareProc, rb_ary_entry(array, i), rb_ary_entry(array, i + 1)));
        }
      }
    }
  }
  return self;
}

VALUE hashUpdate(VALUE self, VALUE key, VALUE val) {
  Hash* hash = rubyCast<Hash>(self);
  auto it = hash->container.find(key);
  if (it != hash->container.end())
    hash->container.replace(it, member(hash->compareProc, key, val));
  else
    hash->container.insert(member(hash->compareProc, key, val));
  return self;
}

VALUE hashReplace(VALUE self, VALUE key, VALUE val) {
  Hash* hash = rubyCast<Hash>(self);
  auto it = hash->container.find(key);
  if (it != hash->container.end()) {
    auto valBackup = it->val;
    hash->container.replace(it, member(hash->compareProc, key, val));
    return valBackup;
  }
  else {
    hash->container.insert(member(hash->compareProc, key, val));
    return Qnil;
  }
}

VALUE hashGet(VALUE self, VALUE key) {
  Hash* hash = rubyCast<Hash>(self);
  auto it = hash->container.find(key);
  if (it == hash->container.end()) {
    return Qnil;
  }
  else {
    return it->val;
  }
}

VALUE hashEach(VALUE self) {
  if (! rb_block_given_p()) {
    // TODO: return Enumerator
    rb_raise(rb_eArgError, "Expected block");
  }
  else {
    Hash* hash = rubyCast<Hash>(self);
    for (auto const& member : hash->container.get<1>()) {
      rb_yield_values(2, member.key, member.val);
    }
  }

  return Qnil;
}

VALUE hashToString(VALUE self) {
  std::stringstream str;
  str << "RbLovely::SortedHash {";
  Hash* hash = rubyCast<Hash>(self);
  if (! hash->container.empty()) {
    auto& idx = hash->container.get<1>();
    auto it = idx.begin();
    str << ' ' << toS(it->key) << " => " << toS(it->val);
    for (++it; it != idx.end(); ++it) {
      str << ", " << toS(it->key) << " => " << toS(it->val);
    }
  }
  str << " }";

  auto stlString = str.str();
  return rb_str_new(stlString.data(), stlString.size());
}

VALUE hashFirst(VALUE self) {
  Hash* hash = rubyCast<Hash>(self);
  return hash->container.empty() ? Qnil : hash->container.get<1>().begin()->val;
}

VALUE hashLast(VALUE self) {
  Hash* hash = rubyCast<Hash>(self);
  if (hash->container.empty())
    return Qnil;

  auto last = hash->container.get<1>().end();
  --last;
  return last->val;
}

VALUE hashMutatingDelete(VALUE self, VALUE toDelete) {
  Hash* hash = rubyCast<Hash>(self);
  auto it = hash->container.find(toDelete);
  if (it == hash->container.end()) {
    return Qnil;
  }
  else {
    auto valBackup = it->val;
    hash->container.erase(it);
    return valBackup;
  }
}

VALUE hashShift(VALUE self) {
  Hash* hash = rubyCast<Hash>(self);
  if (hash->container.empty())
   return Qnil;

  auto& idx = hash->container.get<1>();
  auto bak = idx.begin()->val;
  idx.erase(idx.begin());
  return bak;
}

VALUE hashPop(VALUE self) {
  Hash* hash = rubyCast<Hash>(self);
  if (hash->container.empty())
    return Qnil;

  auto& idx = hash->container.get<1>();
  auto last = idx.end();
  --last;
  auto bak = last->val;
  idx.erase(last);
  return bak;
}

VALUE hashHas(VALUE self, VALUE key) {
  Hash* hash = rubyCast<Hash>(self);
  auto it = hash->container.find(key);
  return it == hash->container.end() ? Qfalse : Qtrue;
}

} }

extern "C" {
  using namespace rb_lovely;
  using namespace rb_lovely::hybrid;

  void Init_rb_lovely_hybrid_set() {
    auto rbHash = rb_define_class_under(rbMod, "SortedHash", rb_cObject);
    rb_define_alloc_func(rbHash, rubyAlloc<Hash>);
    rb_include_module(rbHash, rb_const_get(rb_cObject, rb_intern("Enumerable")));

    rb_define_method(rbHash, "initialize", RUBY_METHOD_FUNC(hashInitialize), -1);
    rb_define_method(rbHash, "length", RUBY_METHOD_FUNC(hashLength), 0);
    rb_define_method(rbHash, "[]=", RUBY_METHOD_FUNC(hashUpdate), 2);
    // like []= but return previous value if there was one
    rb_define_method(rbHash, "replace", RUBY_METHOD_FUNC(hashReplace), 2);
    rb_define_method(rbHash, "[]", RUBY_METHOD_FUNC(hashGet), 1);
    rb_define_method(rbHash, "each", RUBY_METHOD_FUNC(hashEach), 0);
    rb_define_method(rbHash, "to_s", RUBY_METHOD_FUNC(hashToString), 0);
    rb_define_method(rbHash, "first", RUBY_METHOD_FUNC(hashFirst), 0);
    rb_define_method(rbHash, "last", RUBY_METHOD_FUNC(hashLast), 0);
    rb_define_method(rbHash, "delete", RUBY_METHOD_FUNC(hashMutatingDelete), 1);
    // rb_define_method(rbHash, "reject!", RUBY_METHOD_FUNC(hashMutatingReject), 0);
    // rb_define_method(rbHash, "reject_first!", RUBY_METHOD_FUNC(hashMutatingRejectFirst), 0);
    // rb_define_method(rbHash, "select!", RUBY_METHOD_FUNC(hashMutatingSelect), 0);
    rb_define_method(rbHash, "shift", RUBY_METHOD_FUNC(hashShift), 0);
    rb_define_method(rbHash, "pop", RUBY_METHOD_FUNC(hashPop), 0);
    // Enumerable would test both key and value for include?
    rb_define_method(rbHash, "include?", RUBY_METHOD_FUNC(hashHas), 1);
    rb_define_method(rbHash, "has_key?", RUBY_METHOD_FUNC(hashHas), 1);
    rb_define_method(rbHash, "key?", RUBY_METHOD_FUNC(hashHas), 1);
    // rb_define_method(rbHash, "has_value?", RUBY_METHOD_FUNC(hashHasValue), 1);
    // rb_define_method(rbHash, "value?", RUBY_METHOD_FUNC(hashHasValue), 1);
  }
}
#endif
