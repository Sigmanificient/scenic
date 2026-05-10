#include "oxwm.h"
#include "../glibc/glibc.h"
#include "../lua/lua.h"

static const pkg *const oxwm_deps[] = {
    &pkgs_glibc,
    &pkgs_lua,
};

const pkg pkgs_oxwm = {
    .name        = "oxwm",
    .version     = "0.11.4",
    .src         = "https://github.com/tonybanters/oxwm/archive/refs/tags/v0.11.4.tar.gz",
    .sha256      = "0000000000000000000000000000000000000000000000000000000000000000",
    .deps        = { .data = oxwm_deps, .len = sizeof(oxwm_deps) / sizeof(oxwm_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_ZIG,
};
