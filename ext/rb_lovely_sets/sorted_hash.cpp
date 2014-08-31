#ifdef HAVE_BOOST_MULTI_INDEX_CONTAINER_HPP
#include "set.hpp"

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
> Set;

VALUE setInitialize(int argc, VALUE *argv, VALUE self) {
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
        Set* set = rubyCast<Set>(self);
        for (auto i = 0; i < len; i += 2) {
          set->insert(member(rb_ary_entry(array, i), rb_ary_entry(array, i + 1)));
        }
      }
    }
  }
  return self;
}

VALUE setUpdate(VALUE self, VALUE key, VALUE val) {
  Set* set = rubyCast<Set>(self);
  // TODO: overwrite value
  auto it = set->find(key);
  if (it != set->end())
    set->replace(it, member(key, val));
  else
    set->insert(member(key, val));
  return self;
}

VALUE setGet(VALUE self, VALUE key) {
  Set* set = rubyCast<Set>(self);
  auto it = set->find(key);
  if (it == set->end()) {
    return Qnil;
  }
  else {
    return it->val;
  }
}

VALUE setEach(VALUE self) {
  if (! rb_block_given_p()) {
    // TODO: return Enumerator
    rb_raise(rb_eArgError, "Expected block");
  }
  else {
    Set* set = rubyCast<Set>(self);
    for (auto const& member : set->get<1>()) {
      rb_yield_values(2, member.key, member.val);
    }
  }

  return Qnil;
}

VALUE setToString(VALUE self) {
  std::stringstream str;
  str << "RbLovelySets::SortedHash {";
  Set* set = rubyCast<Set>(self);
  if (! set->empty()) {
    auto& idx = set->get<1>();
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

VALUE setFirst(VALUE self) {
  Set* set = rubyCast<Set>(self);
  return set->empty() ? Qnil : set->get<1>().begin()->val;
}

VALUE setLast(VALUE self) {
  Set* set = rubyCast<Set>(self);
  if (set->empty())
    return Qnil;

  auto last = set->get<1>().end();
  --last;
  return last->val;
}

VALUE setMutatingDelete(VALUE self, VALUE toDelete) {
  Set* set = rubyCast<Set>(self);
  auto it = set->find(toDelete);
  if (it == set->end()) {
    return Qnil;
  }
  else {
    auto valBackup = it->val;
    set->erase(it);
    return valBackup;
  }
}

} }

extern "C" {
  using namespace rb_lovely_sets;
  using namespace rb_lovely_sets::hybrid;

  void Init_rb_lovely_sets_hybrid_set() {
    auto rbSet = rb_define_class_under(rbMod, "SortedHash", rb_cObject);
    rb_define_alloc_func(rbSet, rubyAlloc<Set>);
    rb_include_module(rbSet, rb_const_get(rb_cObject, rb_intern("Enumerable")));

    rb_define_method(rbSet, "initialize", RUBY_METHOD_FUNC(setInitialize), -1);
    initSet<Set>(rbSet);
    rb_define_method(rbSet, "[]=", RUBY_METHOD_FUNC(setUpdate), 2);
    rb_define_method(rbSet, "[]", RUBY_METHOD_FUNC(setGet), 1);
    rb_define_method(rbSet, "each", RUBY_METHOD_FUNC(setEach), 0);
    rb_define_method(rbSet, "to_s", RUBY_METHOD_FUNC(setToString), 0);
    rb_define_method(rbSet, "first", RUBY_METHOD_FUNC(setFirst), 0);
    rb_define_method(rbSet, "last", RUBY_METHOD_FUNC(setLast), 0);
    rb_define_method(rbSet, "delete", RUBY_METHOD_FUNC(setMutatingDelete), 1);
    // rb_define_method(rbSet, "reject!", RUBY_METHOD_FUNC(setMutatingReject), 0);
    // rb_define_method(rbSet, "reject_first!", RUBY_METHOD_FUNC(setMutatingRejectFirst), 0);
    // rb_define_method(rbSet, "select!", RUBY_METHOD_FUNC(setMutatingSelect), 0);
    // rb_define_method(rbSet, "shift", RUBY_METHOD_FUNC(setShift), 0);
    // rb_define_method(rbSet, "pop", RUBY_METHOD_FUNC(setPop), 0);
    // // Enumerable provides a slower version of this
    // rb_define_method(rbSet, "has_key?", RUBY_METHOD_FUNC(setHas), 1);
    // rb_define_method(rbSet, "include?", RUBY_METHOD_FUNC(setHas), 1);
  }
}
#endif
