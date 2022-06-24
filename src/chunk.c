#include "chunk.h"

chunk chunk_new() {
    return (chunk){.instructions = uint8_t_vec_new(),
                   .constants = value_vec_new()};
}

void chunk_free(chunk *chunk) {
    uint8_t_vec_free(&chunk->instructions);
    value_vec_free(&chunk->constants);
}

void chunk_add_instruction(chunk *chunk, uint8_t instruction) {
    uint8_t_vec_push(&chunk->instructions, instruction);
}

int chunk_add_constant(chunk *chunk, value value) {
    int len = value_vec_len(&chunk->constants);
    value_vec_push(&chunk->constants, value);
    
    return len;
}
