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

#ifndef ARENA_VEC
#error "ARENA_VEC undefined"
#include "int_vec.h"
#define ARENA_VEC int_vec
#endif

typedef struct {
    ARENA_VEC current;
    size_t len;
    size_t cap;
    ARENA_VEC *chunks;
} ARENA;

ARENA JOIN(ARENA, new)(void);

void JOIN(ARENA, free)(ARENA *arena);

void JOIN(ARENA, push_chunk)(ARENA *arena, ARENA_VEC chunk);

ARENA_TYPE *JOIN(ARENA, alloc)(ARENA *arena, ARENA_TYPE t);

#ifndef ARENA_EXTEND
#undef ARENA
#undef ARENA_VEC
#undef ARENA_TYPE
#endif
