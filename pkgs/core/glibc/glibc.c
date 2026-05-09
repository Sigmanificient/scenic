#include "glibc.h"

const pkg pkgs_glibc = {
    .name        = "glibc",
    .version     = "2.40",
    .src         = "https://ftp.gnu.org/gnu/glibc/glibc-2.40.tar.xz",
    .sha256      = "0000000000000000000000000000000000000000000000000000000000000000",
    .deps        = { .data = nullptr, .len = 0 },
    .build_flags = "",
    .build_sys   = BUILD_AUTOTOOLS,
};
