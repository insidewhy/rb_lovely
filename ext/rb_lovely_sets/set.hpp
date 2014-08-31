#ifndef RB_LOVELY_SETS_SET_HPP
#define RB_LOVELY_SETS_SET_HPP

#include "ruby_util.hpp"

namespace rb_lovely_sets {

template <class T>
VALUE setLength(VALUE self) {
  T* set = rubyCast<T>(self);
  return INT2NUM(set->size());
}

template <class T>
static void initSet(VALUE rbSet) {
  rb_define_method(rbSet, "length", RUBY_METHOD_FUNC(setLength<T>), 0);
}

}
#endif
