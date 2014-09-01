#ifndef RB_LOVELY_SETS_UTIL_HPP
#define RB_LOVELY_SETS_UTIL_HPP
#include "ruby.h"

namespace rb_lovely {

extern VALUE rbMod;
extern VALUE cmpMethSym;
extern VALUE equalitySym;
extern VALUE to_sSym;
extern VALUE hashSym;

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

static void initRubyUtil() {
  rbMod = rb_define_module("RbLovely");
  cmpMethSym = rb_intern("<=>");
  equalitySym = rb_intern("==");
  to_sSym = rb_intern("to_s");
  hashSym = rb_intern("hash");
}

}
#endif
