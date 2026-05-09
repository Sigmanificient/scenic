#include "lua.h"
#include "../../core/glibc/glibc.h"

static const pkg *const lua_deps[] = { &pkgs_glibc };

const pkg pkgs_lua = {
    .name        = "lua",
    .version     = "5.4.7",
    .src         = "https://www.lua.org/ftp/lua-5.4.7.tar.gz",
    .sha256      = "0000000000000000000000000000000000000000000000000000000000000000",
    .deps        = { .data = lua_deps, .len = sizeof(lua_deps) / sizeof(lua_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_MAKE,
};
