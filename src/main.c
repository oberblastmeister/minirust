#include "chunk.h"
#include "debug.h"
#include "memory.h"
#include "prelude.h"
#include "uint8_t_vec.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer_wrapper.h"

int main(int argc, const char *argv[]) {
    run_lexer_debug();
    run_calculator();

    chunk chunk = chunk_new();
    int constant_i = chunk_add_constant(&chunk, 1.2);
    chunk_add_instruction(&chunk, OP_CONSTANT);
    chunk_add_instruction(&chunk, constant_i);
    int constant_i2 = chunk_add_constant(&chunk, 123);
    chunk_add_instruction(&chunk, OP_CONSTANT);
    chunk_add_instruction(&chunk, constant_i2);
    chunk_add_instruction(&chunk, OP_NEGATE);
    int constant_i3 = chunk_add_constant(&chunk, 5.6);
    chunk_add_instruction(&chunk, OP_CONSTANT);
    chunk_add_instruction(&chunk, constant_i3);
    int constant_i4 = chunk_add_constant(&chunk, 1.2);
    chunk_add_instruction(&chunk, OP_CONSTANT);
    chunk_add_instruction(&chunk, constant_i4);
    chunk_add_instruction(&chunk, OP_DIVIDE);
    chunk_add_instruction(&chunk, OP_RETURN);
    disassemble_chunk(&chunk, "test chunk");
    cleanup(vm_free) vm vm = vm_new(chunk);
    vm_run(&vm);
    return 0;
}

// void test(int n, char s[n]) {
    
// }
