#include "neovim.h"
#include "../glibc/glibc.h"
#include "../lua/lua.h"

static const pkg *const neovim_deps[] = {
    &pkgs_glibc,
    &pkgs_lua,
};

const pkg pkgs_neovim = {
    .name        = "neovim",
    .version     = "0.10.2",
    .src         = "https://github.com/neovim/neovim/archive/refs/tags/v0.10.2.tar.gz",
    .sha256      = "0000000000000000000000000000000000000000000000000000000000000000",
    .deps        = { .data = neovim_deps,
                     .len = sizeof(neovim_deps) / sizeof(neovim_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_CMAKE,
};
