#ifndef RB_LOVELY_CONTAINER_HPP
#define RB_LOVELY_CONTAINER_HPP

#include "ruby_util.hpp"

namespace rb_lovely {

// TODO: No more toS, provide inspect instead which is what the containers should be using
//       for each member.
auto toS = [](VALUE val) { return RSTRING_PTR(rb_funcall(val, to_sSym, 0)); };

}
#endif
