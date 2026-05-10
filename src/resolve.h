#ifndef SCENIC_RESOLVE_H
#define SCENIC_RESOLVE_H

#include "../include/scenicos.h"
#include "error.h"
#include "arena.h"

typedef struct {
    const pkg  *def;
    const char *store_path;
} resolved;

typedef struct {
    const resolved *data;
    size_t          len;
} resolved_list;

[[nodiscard]] resolve_error resolve(
    arena              *a,
    const system_cfg   *cfg,
    resolved_list      *out);

#endif
