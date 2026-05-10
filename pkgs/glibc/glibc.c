#include "glibc.h"

const pkg pkgs_glibc = {
    .name        = "glibc",
    .version     = "2.40",
    .src         = "https://ftpmirror.gnu.org/gnu/glibc/glibc-2.40.tar.xz",
    .sha256      = "19a890175e9263d748f627993de6f4b1af9cd21e03f080e4bfb3a1fac10205a2",
    .deps        = { .data = NULL, .len = 0 },
    .build_flags = "",
    .build_sys   = BUILD_AUTOTOOLS,
};
