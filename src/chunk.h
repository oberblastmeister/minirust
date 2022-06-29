#ifndef _LOX_CHUNK_H
#define _LOX_CHUNK_H

#include "uint8_t_vec.h"
#include "prelude.h"
#include "value.h"

typedef struct {
    uint8_t_vec instructions;
    value_vec constants;
} chunk;

chunk chunk_new(void);

void chunk_free(chunk *chunk);

void chunk_add_instruction(chunk *chunk, uint8_t instruction);

int chunk_add_constant(chunk *chunk, value value);

#endif
