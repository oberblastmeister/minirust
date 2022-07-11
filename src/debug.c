#include "debug.h"
#include "opcode.h"
#include "string.h"
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

static void constant_instruction(chunk *chunk, int offset) {
    uint8_t constant = uint8_t_vec_index(&chunk->instructions, offset + 1);
    printf("%4d '", constant);
    value_print(value_vec_index(&chunk->constants, constant));
    printf("'");
}

static int read_to_int(uint8_t *p, int n) {
    if (n == 1) {
        uint8_t res;
        memcpy(&res, p, 1);
        return (int)res;
    } else if (n == 2) {
        uint16_t res;
        memcpy(&res, p, 2);
        return (int)res;
    } else if (n == 4) {
        uint32_t res;
        memcpy(&res, p, 4);
        return (int)res;
    } else {
        printf("Bytes must be multiple of two\n");
        return 0;
    }
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
    int arg_bytes = op_arg_bytes[instruction];
    switch (instruction) {
    case OP_CONST: {
        constant_instruction(chunk, offset);
        break;
    }
    default: {
        if (arg_bytes > 0) {
            int arg = read_to_int(
                uint8_t_vec_get_ptr(&chunk->instructions, offset + 1),
                arg_bytes);
            printf("%4d", arg);
        }
        break;
    }
    }
    printf("\n");
    return offset + 1 + arg_bytes;
}
