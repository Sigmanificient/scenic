#ifndef NB_FETCH_H
#define NB_FETCH_H

#include "../include/nbos.h"
#include "error.h"

[[nodiscard]] fetch_error fetch(
    const pkg  *p,
    const char *dest_path);

#endif
