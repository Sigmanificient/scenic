#include "linux.h"

const pkg pkgs_linux = {
    .name        = "linux",
    .version     = "6.12",
    .src         = "https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.12.tar.xz",
    .sha256      = "0000000000000000000000000000000000000000000000000000000000000000",
    .deps        = { .data = nullptr, .len = 0 },
    .build_flags = "",
    .build_sys   = BUILD_MAKE,
};
