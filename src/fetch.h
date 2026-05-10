#ifndef SCENIC_FETCH_H
#define SCENIC_FETCH_H

#include "../include/scenicos.h"
#include "error.h"

[[nodiscard]] fetch_error fetch(
    const pkg  *p,
    const char *dest_path);

#endif
