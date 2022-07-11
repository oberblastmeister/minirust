#include "macro_util.h"
#include "prelude.h"
#include "slice.h"
#include <stdlib.h>

#ifndef ARENA
#define ARENA JOIN(ARENA_TYPE, arena)
#endif

#define ARENA_CHUNK JOIN(ARENA_TYPE, arena_chunk)

#ifndef ARENA_TYPE
#error "ARENA_TYPE undefined"
// do this just to get better support for clangd
// the file will already fail to compile
#define ARENA_TYPE int
#endif

typedef struct {
    size_t len;
    size_t cap;
    ARENA_TYPE *data;
} ARENA_CHUNK;

typedef struct {
    ARENA_CHUNK current;
    size_t len;
    size_t cap;
    slice *chunks;
} ARENA;

ARENA JOIN(ARENA, new)(void);

void JOIN(ARENA, free)(ARENA *arena);

void JOIN(ARENA, push_slice)(ARENA *arena, slice slice);

ARENA_TYPE *JOIN(ARENA, alloc)(ARENA *arena, ARENA_TYPE t);

#ifndef ARENA_EXTEND
#undef ARENA
#undef ARENA_TYPE
#undef ARENA_TYPE_FREE
#undef ARENA_TYPE_COPY
#endif
