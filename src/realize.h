#ifndef SCENIC_REALIZE_H
#define SCENIC_REALIZE_H

#include "../include/scenicos.h"
#include "arena.h"
#include "error.h"
#include "resolve.h"

realize_error realize(
    arena              *a,
    const pkg_refs     *all_pkgs,
    const resolved_list *resolved);

#endif
