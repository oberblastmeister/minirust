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

typedef struct {
    size_t len;
    // cannot be accessed when inside chunks array
    size_t cap;
    ARENA_TYPE *data;
} ARENA_CHUNK;

typedef struct {
    ARENA_CHUNK current;
    size_t len;
    size_t cap;
    slice *chunks;
} ARENA;
#endif

ARENA_CHUNK _JOIN(ARENA_CHUNK, new_with_cap)(size_t cap) {
    return (ARENA_CHUNK){
        0,
        cap,
        (ARENA_TYPE *)malloc(sizeof(ARENA_TYPE) * cap),
    };
}

void _JOIN(ARENA_CHUNK, free)(ARENA_CHUNK *chunk) {
#ifdef ARENA_TYPE_FREE
    for (size_t i = 0; i < chunk->len; i++) {
        ARENA_TYPE_FREE(&chunk->data[i]);
    }
#endif
    free(chunk->data);
}

ARENA JOIN(ARENA, new)(void) {
    return (ARENA){_JOIN(ARENA_CHUNK, new_with_cap)(1024 / sizeof(ARENA_TYPE)),
                   0, 0, NULL};
}

void JOIN(ARENA, free)(ARENA *arena) {
    for (size_t i = 0; i < arena->len; i++) {
        _JOIN(ARENA_CHUNK, free)(&arena->chunks[i]);
    }
    free(arena->chunks);
}

void _JOIN(ARENA, grow_chunks)(ARENA *arena, size_t i) {
    size_t cap = max((size_t)8, max(arena->cap * 2, arena->len + i));
    if (arena->cap == 0) {
        arena->cap = cap;
        arena->chunks = (ARENA_CHUNK *)malloc(sizeof(ARENA_CHUNK) * cap);
        return;
    }
    arena->cap = cap;
    arena->chunks =
        (ARENA_CHUNK *)realloc(arena->chunks, sizeof(ARENA_CHUNK) * cap);
}

void _JOIN(ARENA, reserve_chunks)(ARENA *arena, size_t i) {
    if (arena->len + i > arena->cap) {
        _JOIN(ARENA, grow_chunks)(arena, i);
    }
}

void JOIN(ARENA, push_slice)(ARENA *arena, slice slice) {
    _JOIN(ARENA, reserve_chunks)(arena, 1);
    arena->chunks[arena->len] = slice;
    arena->len++;
}

ARENA_TYPE *JOIN(ARENA, alloc)(ARENA *arena, ARENA_TYPE t) {
    if (arena->current.len == arena->current.cap) {
        JOIN(ARENA, push_slice)
        (arena, (slice){arena->current.len, arena->current.data});
        arena->current =
            _JOIN(ARENA_CHUNK, new_with_cap)(arena->current.cap * 2);
    }
    arena->current.data[arena->current.len] = t;
    arena->current.len++;
    return &arena->current.data[arena->current.len - 1];
}

#ifndef ARENA_EXTEND
#undef ARENA
#undef ARENA_TYPE
#undef ARENA_TYPE_FREE
#undef ARENA_TYPE_COPY
#endif
