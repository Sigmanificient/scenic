#ifndef SCENIC_ACTIVATE_H
#define SCENIC_ACTIVATE_H

#include "../include/scenicos.h"
#include "arena.h"
#include "resolve.h"

#include <stdint.h>

[[nodiscard]] int activate(
    arena               *a,
    const system_cfg    *cfg,
    const resolved_list *resolved,
    uint32_t             generation);

[[nodiscard]] int activate_rollback(arena *a);

#endif
