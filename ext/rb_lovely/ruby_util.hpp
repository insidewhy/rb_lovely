#ifndef RB_LOVELY_UTIL_HPP
#define RB_LOVELY_UTIL_HPP
#include "ruby.h"

namespace rb_lovely {

extern VALUE rbMod;
extern VALUE cmpMethSym;
extern VALUE hashEqualitySym;
extern VALUE inspectSym;
extern VALUE hashSym;
extern VALUE callSym;
extern VALUE compareSym;

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

typedef void (*MarkFunction)(void *);
template <class T, MarkFunction M>
VALUE rubyAlloc(VALUE klass) {
  return Data_Wrap_Struct(klass, M, rubyDelete<T>, new T);
}

static void initRubyUtil() {
  rbMod = rb_define_module("RbLovely");
  cmpMethSym = rb_intern("<=>");
  hashEqualitySym = rb_intern("eql?");
  inspectSym = rb_intern("inspect");
  hashSym = rb_intern("hash");
  callSym = rb_intern("call");
  compareSym = ID2SYM(rb_intern("compare"));
}

}
#endif
