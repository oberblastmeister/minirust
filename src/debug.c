#include "debug.h"
#include "opcode.h"
#include "uint8_t_vec.h"
#include "value.h"
#include <stdio.h>

void disassemble_chunk(chunk *chunk, const char *name) {
    printf("== %s ==\n", name);

    for (size_t offset = 0; offset < uint8_t_vec_len(&chunk->instructions);) {
        offset = disassemble_instruction(chunk, offset);
    }
}

static int simple_instruction(const char *name, int offset) {
    puts(name);
    return offset + 1;
}

static constant_instruction(chunk *chunk, int offset) {
    uint8_t constant = uint8_t_vec_index(&chunk->instructions, offset + 1);
    printf("%4d '", constant);
    value_print(value_vec_index(&chunk->constants, constant));
    printf("'");
}

int disassemble_instruction(chunk *chunk, int offset) {
    printf("%04d ", offset);

    uint8_t instruction = uint8_t_vec_index(&chunk->instructions, offset);
    if (instruction < op_amount) {
        printf("%-16s", op_names[instruction]);
    } else {
        printf("<unknown>");
        return offset + 1;
    }
    int args = op_args[instruction];
    switch (instruction) {
    case OP_CONST: {
        constant_instruction(chunk, offset);
        break;
    }
    case OP_POP_N: {
        break;
    }
    case OP_LOAD_LOCAL: {
        break;
    }
    case OP_STORE_LOCAL: {
        break;
    }
    default: {
        break;
    }
    }
    printf("\n");
    return offset + 1 + args;
}
