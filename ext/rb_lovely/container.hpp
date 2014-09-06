#ifndef RB_LOVELY_CONTAINER_HPP
#define RB_LOVELY_CONTAINER_HPP

#include "ruby_util.hpp"

namespace rb_lovely {

auto rbInspect = [](VALUE val) { return RSTRING_PTR(rb_funcall(val, inspectSym, 0)); };

}
#endif
