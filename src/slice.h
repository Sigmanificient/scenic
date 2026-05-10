#ifndef SCENIC_SLICE_H
#define SCENIC_SLICE_H

#include <stddef.h>

#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))
#define SLICE_FROM_ARRAY(a) { .data = (a), .len = ARRAY_LEN(a) }
#define SLICE_EMPTY { .data = nullptr, .len = 0 }

#endif
