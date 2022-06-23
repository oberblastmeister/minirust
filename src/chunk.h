#ifndef CHUNK_H
#define CHUNK_H

#include "prelude.h"

typedef enum {
    OP_RETURN,
} op_code;

typedef struct {
    int count;
    int capacity;
    uint8_t *code;
} chunk;

chunk chunk_new();

void chunk_init(chunk *chunk);

void chunk_write(chunk *chunk, uint8_t byte);

void chunk_free(chunk *chunk);

#endif
