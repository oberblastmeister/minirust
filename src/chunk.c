#include "chunk.h"
#include "memory.h"
#include "prelude.h"
#include <stdlib.h>

chunk chunk_new() {
    chunk chunk;
    chunk_init(&chunk);
    return chunk;
}

void chunk_init(chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
}

void chunk_write(chunk *chunk, uint8_t byte) {
    if (chunk->capacity == chunk->count) {
        int old_capacity = chunk->capacity;
        chunk->capacity = old_capacity < 8 ? 8 : old_capacity * 2;
        chunk->code =
            reallocate(chunk->code, sizeof(uint8_t) * chunk->capacity);
    }
    chunk->code[chunk->count] = byte;
    chunk->count++;
}

void chunk_free(chunk *chunk) {
    free(chunk->code);
    chunk_init(chunk);
}
