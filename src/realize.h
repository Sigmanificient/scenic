#ifndef NB_REALIZE_H
#define NB_REALIZE_H

#include "../include/nbos.h"
#include "arena.h"
#include "error.h"
#include "resolve.h"

[[nodiscard]] realize_error realize(
    arena              *a,
    const pkg_refs     *all_pkgs,
    const resolved_list *resolved);

#endif
