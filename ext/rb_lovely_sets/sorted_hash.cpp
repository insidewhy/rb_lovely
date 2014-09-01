#ifdef HAVE_BOOST_MULTI_INDEX_CONTAINER_HPP
#include "container.hpp"

#include "ruby_util.hpp"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

namespace rb_lovely_sets { namespace hybrid {

struct member {
  bool operator<(member const& rhs) const {
    auto cmpVal = rb_funcall(val, cmpMethSym, 1, rhs.val);
    return NUM2INT(cmpVal) < 0;
  }

  bool operator==(member const& rhs) const {
    auto equalityVal = rb_funcall(key, equalitySym, 1, rhs.key);
    return RTEST(equalityVal);
  }

  // also cache as two element array?
  member(VALUE _key, VALUE _val) : key(_key), val(_val) {}

  VALUE key;
  VALUE val;
};

std::size_t hash_value(member const& member) {
  // TODO: something better?
  return reinterpret_cast<std::size_t>(&member);
  // return NUM2INT(rb_funcall(member.val, hashSym, 0));
}

namespace mi = boost::multi_index;

typedef boost::multi_index_container<
  member,
  mi::indexed_by<
    mi::hashed_unique< mi::member<member, VALUE, &member::key> >,
    mi::ordered_non_unique< mi::identity<member> >
  >
> Hash;

VALUE hashInitialize(int argc, VALUE *argv, VALUE self) {
  if (argc == 1) {
    auto array = rb_check_array_type(argv[0]);
    if (array == Qnil) {
      rb_raise(rb_eArgError, "Expected array");
    }
    else {
      auto len = RARRAY_LEN(array);
      if (len % 2 != 0) {
        rb_raise(rb_eArgError, "Expected even number of parameters");
      }
      else {
        Hash* hash = rubyCast<Hash>(self);
        for (auto i = 0; i < len; i += 2) {
          hash->insert(member(rb_ary_entry(array, i), rb_ary_entry(array, i + 1)));
        }
      }
    }
  }
  return self;
}

VALUE hashUpdate(VALUE self, VALUE key, VALUE val) {
  Hash* hash = rubyCast<Hash>(self);
  // TODO: overwrite value
  auto it = hash->find(key);
  if (it != hash->end())
    hash->replace(it, member(key, val));
  else
    hash->insert(member(key, val));
  return self;
}

VALUE hashGet(VALUE self, VALUE key) {
  Hash* hash = rubyCast<Hash>(self);
  auto it = hash->find(key);
  if (it == hash->end()) {
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
    for (auto const& member : hash->get<1>()) {
      rb_yield_values(2, member.key, member.val);
    }
  }

  return Qnil;
}

VALUE hashToString(VALUE self) {
  std::stringstream str;
  str << "RbLovelySets::SortedHash {";
  Hash* hash = rubyCast<Hash>(self);
  if (! hash->empty()) {
    auto& idx = hash->get<1>();
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
  return hash->empty() ? Qnil : hash->get<1>().begin()->val;
}

VALUE hashLast(VALUE self) {
  Hash* hash = rubyCast<Hash>(self);
  if (hash->empty())
    return Qnil;

  auto last = hash->get<1>().end();
  --last;
  return last->val;
}

VALUE hashMutatingDelete(VALUE self, VALUE toDelete) {
  Hash* hash = rubyCast<Hash>(self);
  auto it = hash->find(toDelete);
  if (it == hash->end()) {
    return Qnil;
  }
  else {
    auto valBackup = it->val;
    hash->erase(it);
    return valBackup;
  }
}

VALUE hashShift(VALUE self) {
  Hash* hash = rubyCast<Hash>(self);
  if (hash->empty())
   return Qnil;

  auto& idx = hash->get<1>();
  auto bak = idx.begin()->val;
  idx.erase(idx.begin());
  return bak;
}

VALUE hashPop(VALUE self) {
  Hash* hash = rubyCast<Hash>(self);
  if (hash->empty())
    return Qnil;

  auto& idx = hash->get<1>();
  auto last = idx.end();
  --last;
  auto bak = last->val;
  idx.erase(last);
  return bak;
}

VALUE hashHas(VALUE self, VALUE key) {
  Hash* hash = rubyCast<Hash>(self);
  auto it = hash->find(key);
  return it == hash->end() ? Qfalse : Qtrue;
}

} }

extern "C" {
  using namespace rb_lovely_sets;
  using namespace rb_lovely_sets::hybrid;

  void Init_rb_lovely_sets_hybrid_set() {
    auto rbHash = rb_define_class_under(rbMod, "SortedHash", rb_cObject);
    rb_define_alloc_func(rbHash, rubyAlloc<Hash>);
    rb_include_module(rbHash, rb_const_get(rb_cObject, rb_intern("Enumerable")));

    rb_define_method(rbHash, "initialize", RUBY_METHOD_FUNC(hashInitialize), -1);
    initSet<Hash>(rbHash);
    rb_define_method(rbHash, "[]=", RUBY_METHOD_FUNC(hashUpdate), 2);
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
  }
}
#endif
