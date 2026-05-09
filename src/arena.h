#ifndef NB_ARENA_H
#define NB_ARENA_H

#include <stddef.h>

typedef struct arena arena;

[[nodiscard]] arena *arena_create(size_t initial_capacity);
void  arena_destroy(arena *a);

[[nodiscard]] void *arena_alloc(arena *a, size_t size, size_t align);
[[nodiscard]] char *arena_strdup(arena *a, const char *s);
[[nodiscard]] char *arena_sprintf(arena *a, const char *fmt, ...);

void arena_reset(arena *a);

size_t arena_used(const arena *a);
size_t arena_capacity(const arena *a);

#endif
