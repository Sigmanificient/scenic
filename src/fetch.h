#ifndef SCENIC_FETCH_H
#define SCENIC_FETCH_H

#include "scenicos.h"
#include "error.h"

fetch_error fetch(
    const pkg  *p,
    const char *dest_path);

#endif
