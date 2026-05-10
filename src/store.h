#ifndef NB_STORE_H
#define NB_STORE_H

#include "../include/nbos.h"
#include "arena.h"
#include "resolve.h"

#define NB_STORE_ROOT "/nb/store"

[[nodiscard]] const char *store_path_compute(
    arena              *a,
    const pkg          *p,
    const resolved     *resolved_pkgs,
    size_t              n_resolved);

[[nodiscard]] bool store_path_exists(const char *store_path);

[[nodiscard]] int store_install(const char *temp_path, const char *store_path);

[[nodiscard]] int store_remove(const char *store_path);

#endif
