#ifndef RB_LOVELY_SETS_SET_HPP
#define RB_LOVELY_SETS_SET_HPP

#include "ruby_util.hpp"

namespace rb_lovely {

template <class T>
VALUE containerLength(VALUE self) {
  T* set = rubyCast<T>(self);
  return INT2NUM(set->size());
}

template <class T>
static void initSet(VALUE rbSet) {
  rb_define_method(rbSet, "length", RUBY_METHOD_FUNC(containerLength<T>), 0);
}

auto toS = [](VALUE val) { return RSTRING_PTR(rb_funcall(val, to_sSym, 0)); };

}
#endif
