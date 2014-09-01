#include "ruby_util.hpp"

namespace rb_lovely {
  // extern stuff from ruby_util
  VALUE rbMod;
  VALUE cmpMethSym;
  VALUE hashEqualitySym;
  VALUE to_sSym;
  VALUE hashSym;
  VALUE compareSym;
  VALUE callSym;
}

extern "C" {
  using namespace rb_lovely;

  extern void Init_rb_lovely_sorted_set();
#ifdef HAVE_BOOST_MULTI_INDEX_CONTAINER_HPP
  extern void Init_rb_lovely_hybrid_set();
#endif

  void Init_rb_lovely() {
    ruby_init();
    ruby_init_loadpath();

    initRubyUtil();

    Init_rb_lovely_sorted_set();
#ifdef HAVE_BOOST_MULTI_INDEX_CONTAINER_HPP
    Init_rb_lovely_hybrid_set();
#endif

    // i saw this somewhere... but it dumps core... so um...
    // ruby_finalize();
  }
}
