#include "container.hpp"

#include "ruby_util.hpp"

#include <set>
#include <sstream>

namespace rb_lovely { namespace ordered {

struct Compare {
  bool operator()(VALUE const& lhs, VALUE const& rhs);
};

typedef std::set<VALUE, Compare> Set;

bool Compare::operator()(VALUE const& lhs, VALUE const& rhs) {
  auto cmpVal = rb_funcall(lhs, cmpMethSym, 1, rhs);
  return NUM2INT(cmpVal) < 0;
}

VALUE setInitialize(int argc, VALUE *argv, VALUE self) {
  if (argc == 1) {
    auto array = rb_check_array_type(argv[0]);
    if (array == Qnil) {
      rb_raise(rb_eArgError, "Expected array");
    }
    else {
      Set* set = rubyCast<Set>(self);
      auto len = RARRAY_LEN(array);
      for (auto i = 0; i < len; ++i) {
        set->insert(rb_ary_entry(array, i));
      }
    }
  }
  return self;
}

VALUE setAdd(VALUE self, VALUE val) {
  Set* set = rubyCast<Set>(self);
  set->insert(val);
  return self;
}

VALUE setEach(VALUE self) {
  if (! rb_block_given_p()) {
    // TODO: return Enumerator
    rb_raise(rb_eArgError, "Expected block");
  }
  else {
    Set* set = rubyCast<Set>(self);
    for (auto const& val : *set) {
      rb_yield(val);
    }
  }

  return Qnil;
}

VALUE setToString(VALUE self) {
  std::stringstream str;
  str << "RbLovely::SortedSet {";
  Set* set = rubyCast<Set>(self);
  if (! set->empty()) {
    auto it = set->begin();
    str << ' ' << toS(*it);
    for (++it; it != set->end(); ++it) {
      str << ", " << toS(*it);
    }
  }
  str << " }";

  auto stlString = str.str();
  return rb_str_new(stlString.data(), stlString.size());
}

VALUE setFirst(VALUE self) {
  Set* set = rubyCast<Set>(self);

  return set->empty() ? Qnil : *set->begin();
}

VALUE setLast(VALUE self) {
  Set* set = rubyCast<Set>(self);
  if (set->empty())
    return Qnil;

  auto last = set->end();
  --last;
  return *last;
}

VALUE setMutatingDelete(VALUE self, VALUE toDelete) {
  Set* set = rubyCast<Set>(self);
  auto it = set->find(toDelete);
  if (it == set->end()) {
    return Qnil;
  }
  else {
    auto valBackup = *it;
    set->erase(it);
    return valBackup;
  }
}

VALUE setMutatingReject(VALUE self) {
  if (! rb_block_given_p()) {
    rb_raise(rb_eArgError, "Expected block");
  }
  else {
    Set* set = rubyCast<Set>(self);
    for (auto it = set->begin(); it != set->end();) {
      auto predicateRetVal = rb_yield(*it);
      if (RTEST(predicateRetVal))
        it = set->erase(it);
      else
        ++it;
    }
  }
  return self;
}

VALUE setMutatingRejectFirst(VALUE self) {
  if (! rb_block_given_p()) {
    rb_raise(rb_eArgError, "Expected block");
  }
  else {
    Set* set = rubyCast<Set>(self);
    for (auto it = set->begin(); it != set->end(); ++it) {
      auto predicateRetVal = rb_yield(*it);
      if (RTEST(predicateRetVal)) {
        auto valBackup = *it;
        set->erase(it);
        return valBackup;
      }
    }
  }
  return Qnil;
}

VALUE setMutatingSelect(VALUE self) {
  if (! rb_block_given_p()) {
    rb_raise(rb_eArgError, "Expected block");
    return self;
  }
  else {
    Set* set = rubyCast<Set>(self);
    for (auto it = set->begin(); it != set->end();) {
      auto predicateRetVal = rb_yield(*it);
      if (! RTEST(predicateRetVal))
        it = set->erase(it);
      else
        ++it;
    }
  }
  return self;
}

VALUE setShift(VALUE self) {
  Set* set = rubyCast<Set>(self);
  if (set->empty())
   return Qnil;

  auto bak = *set->begin();
  set->erase(set->begin());
  return bak;
}

VALUE setPop(VALUE self) {
  Set* set = rubyCast<Set>(self);
  if (set->empty())
    return Qnil;

  auto last = set->end();
  --last;
  auto bak = *last;
  set->erase(last);
  return bak;
}

VALUE setHas(VALUE self, VALUE val) {
  Set* set = rubyCast<Set>(self);
  auto it = set->find(val);
  return it == set->end() ? Qfalse : Qtrue;
}

} } // end namespace

extern "C" {
  using namespace rb_lovely;
  using namespace rb_lovely::ordered;

  void Init_rb_lovely_sorted_set() {
    auto rbSet = rb_define_class_under(rbMod, "SortedSet", rb_cObject);
    rb_define_alloc_func(rbSet, rubyAlloc<Set>);
    rb_include_module(rbSet, rb_const_get(rb_cObject, rb_intern("Enumerable")));

    rb_define_method(rbSet, "initialize", RUBY_METHOD_FUNC(setInitialize), -1);
    initSet<Set>(rbSet);
    rb_define_method(rbSet, "add", RUBY_METHOD_FUNC(setAdd), 1);
    rb_define_method(rbSet, "<<", RUBY_METHOD_FUNC(setAdd), 1);
    rb_define_method(rbSet, "each", RUBY_METHOD_FUNC(setEach), 0);
    rb_define_method(rbSet, "to_s", RUBY_METHOD_FUNC(setToString), 0);
    rb_define_method(rbSet, "first", RUBY_METHOD_FUNC(setFirst), 0);
    rb_define_method(rbSet, "last", RUBY_METHOD_FUNC(setLast), 0);
    rb_define_method(rbSet, "delete", RUBY_METHOD_FUNC(setMutatingDelete), 1);
    rb_define_method(rbSet, "reject!", RUBY_METHOD_FUNC(setMutatingReject), 0);
    rb_define_method(rbSet, "reject_first!", RUBY_METHOD_FUNC(setMutatingRejectFirst), 0);
    rb_define_method(rbSet, "select!", RUBY_METHOD_FUNC(setMutatingSelect), 0);
    rb_define_method(rbSet, "shift", RUBY_METHOD_FUNC(setShift), 0);
    rb_define_method(rbSet, "pop", RUBY_METHOD_FUNC(setPop), 0);
    // Enumerable provides a slower version of this
    rb_define_method(rbSet, "include?", RUBY_METHOD_FUNC(setHas), 1);
  }
}
