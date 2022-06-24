#include "debug.h"
#include "value.h"
#include "uint8_t_vec.h"
#include <stdio.h>

void disassemble_chunk(chunk *chunk, const char *name) {
    printf("== %s ==\n", name);

    for (size_t offset = 0; offset < uint8_t_vec_len(&chunk->instructions);) {
        offset = disassemble_instruction(chunk, offset);
    }
}

static int constant_instruction(const char *name, chunk *chunk, int offset) {
    uint8_t constant = uint8_t_vec_index(&chunk->instructions, offset + 1);
    printf("%-16s %4d '", name, constant);
    value_print(value_vec_index(&chunk->constants, constant));
    printf("'\n");
    return offset + 2;
}

int disassemble_instruction(chunk *chunk, int offset) {
    printf("%04d ", offset);

    uint8_t instruction = uint8_t_vec_index(&chunk->instructions, offset);
    switch (instruction) {
    case OP_CONSTANT:
        return constant_instruction("OP_CONSTANT", chunk, offset);
    case OP_RETURN:
        puts("OP_RETURN");
        return offset + 1;
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}
