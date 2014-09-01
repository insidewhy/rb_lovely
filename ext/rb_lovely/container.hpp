#ifndef RB_LOVELY_SETS_CONTAINER_HPP
#define RB_LOVELY_SETS_CONTAINER_HPP

#include "ruby_util.hpp"

namespace rb_lovely {

auto toS = [](VALUE val) { return RSTRING_PTR(rb_funcall(val, to_sSym, 0)); };

}
#endif
