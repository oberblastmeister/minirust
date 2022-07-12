#include "bits.h"
#include "macro_util.h"
#include "prelude.h"
#include "ptr.h"
#include "slice.h"
#include <stdlib.h>
#include <string.h>

#ifndef ARENA
#define ARENA JOIN(ARENA_TYPE, arena)
#endif

#define ARENA_CHUNK JOIN(ARENA_TYPE, arena_chunk)

#ifndef ARENA_TYPE
#error "ARENA_TYPE undefined"
// do this just to get better support for clangd
// the file will already fail to compile
#define ARENA_TYPE int

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
#endif

ARENA JOIN(ARENA, new)(void) {
    return (ARENA){JOIN(ARENA_VEC, new_with_cap)(1024 / sizeof(ARENA_TYPE)), 0,
                   0, NULL};
}

void JOIN(ARENA, free)(ARENA *arena) {
    for (size_t i = 0; i < arena->len; i++) {
        JOIN(ARENA_VEC, free)(&arena->chunks[i]);
    }
    free(arena->chunks);
}

void _JOIN(ARENA, grow_chunks)(ARENA *arena, size_t i) {
    size_t cap = max((size_t)8, max(arena->cap * 2, arena->len + i));
    if (arena->cap == 0) {
        arena->cap = cap;
        arena->chunks = (ARENA_VEC *)malloc(sizeof(ARENA_VEC) * cap);
        return;
    }
    arena->cap = cap;
    arena->chunks =
        (ARENA_VEC *)realloc(arena->chunks, sizeof(ARENA_VEC) * cap);
}

void _JOIN(ARENA, reserve_chunks)(ARENA *arena, size_t i) {
    if (arena->len + i > arena->cap) {
        _JOIN(ARENA, grow_chunks)(arena, i);
    }
}

void JOIN(ARENA, push_chunk)(ARENA *arena, ARENA_VEC chunk) {
    _JOIN(ARENA, reserve_chunks)(arena, 1);
    arena->chunks[arena->len] = chunk;
    arena->len++;
}

ARENA_TYPE *JOIN(ARENA, alloc)(ARENA *arena, ARENA_TYPE t) {
    if (arena->current.len == arena->current.cap) {
        JOIN(ARENA, push_chunk)(arena, arena->current);
        arena->current = JOIN(ARENA_VEC, new_with_cap)(arena->current.cap * 2);
    }
    arena->current.data[arena->current.len] = t;
    arena->current.len++;
    return &arena->current.data[arena->current.len - 1];
}

#ifndef ARENA_EXTEND
#undef ARENA
#undef ARENA_VEC
#undef ARENA_TYPE
#endif
