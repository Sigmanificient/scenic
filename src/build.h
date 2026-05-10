#ifndef NB_BUILD_H
#define NB_BUILD_H

#include "../include/nbos.h"
#include "arena.h"
#include "error.h"
#include "resolve.h"

[[nodiscard]] realize_error build_pkg(
    arena              *a,
    const pkg          *p,
    const pkg_refs     *all_pkgs,
    const resolved     *resolved_pkgs,
    size_t              pkg_idx
);

#endif
