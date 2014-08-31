#include "ruby.h"

#include <iostream>
#include <set>

namespace rb_lovely_sorted_set {

// Scaffolding stuff for ruby {
template <class T>
void rubyDelete(T *obj) {
  delete obj;
}

template <class T>
T *rubyCast(VALUE rbObj) {
  T *obj;
  Data_Get_Struct(rbObj, T, obj);
  return obj;
}

template <class T>
VALUE rubyAlloc(VALUE klass) {
  return Data_Wrap_Struct(klass, 0, rubyDelete<T>, new T);
}

VALUE rubyIdentity(VALUE self) {
  return self;
}
// } end scaffolding

VALUE rbMod;
VALUE rbSet;
VALUE cmpMethSym;

struct Compare {
  bool operator()(VALUE const& lhs, VALUE const& rhs);
};

typedef std::set<VALUE, Compare> Set;

bool Compare::operator()(VALUE const& lhs, VALUE const& rhs) {
  auto val = rb_funcall(lhs, cmpMethSym, 1, rhs);
  return NUM2INT(val) < 0;
}

VALUE setAdd(VALUE self, VALUE val) {
  Set* set = rubyCast<Set>(self);
  set->insert(val);
  return self;
}

VALUE setLength(VALUE self) {
  Set* set = rubyCast<Set>(self);
  return INT2NUM(set->size());
}

VALUE setEach(VALUE self) {
  Set* set = rubyCast<Set>(self);
  if (! rb_block_given_p())
    rb_raise(rb_eArgError, "Expected block");

  for (auto const& val : *set) {
    rb_yield(val);
  }
}

} // end namespace

extern "C" {
  using namespace rb_lovely_sorted_set;
  void Init_rb_lovely_sorted_set() {
    ruby_init();
    ruby_init_loadpath();

    cmpMethSym = rb_intern("<=>");

    rbMod = rb_define_module("RbLovelySorted");
    rbSet = rb_define_class_under(rbMod, "Set", rb_cObject);
    rb_define_alloc_func(rbSet, rubyAlloc<Set>);
    rb_include_module(rbSet, rb_const_get(rb_cObject, rb_intern("Enumerable")));

    rb_define_method(rbSet, "initialize", RUBY_METHOD_FUNC(rubyIdentity), 0);
    rb_define_method(rbSet, "add", RUBY_METHOD_FUNC(setAdd), 1);
    rb_define_method(rbSet, "length", RUBY_METHOD_FUNC(setLength), 0);
    rb_define_method(rbSet, "each", RUBY_METHOD_FUNC(setEach), 0);

    // i saw this somewhere... but it dumps core... so um...
    // ruby_finalize();
  }
}
