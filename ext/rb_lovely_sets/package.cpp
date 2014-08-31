#include "ruby_util.hpp"

namespace rb_lovely_sets {
  // extern stuff from ruby_util
  VALUE rbMod;
  VALUE cmpMethSym;
  VALUE equalitySym;
  VALUE to_sSym;
  VALUE hashSym;
}

extern "C" {
  using namespace rb_lovely_sets;

  extern void Init_rb_lovely_sets_ordered_set();
#ifdef HAVE_BOOST_MULTI_INDEX_CONTAINER_HPP
  extern void Init_rb_lovely_sets_hybrid_set();
#endif

  void Init_rb_lovely_sets() {
    ruby_init();
    ruby_init_loadpath();

    initRubyUtil();

    Init_rb_lovely_sets_ordered_set();
#ifdef HAVE_BOOST_MULTI_INDEX_CONTAINER_HPP
    Init_rb_lovely_sets_hybrid_set();
#endif

    // i saw this somewhere... but it dumps core... so um...
    // ruby_finalize();
  }
}
