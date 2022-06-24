#include "uint8_t_vec.h"
#include "chunk.h"
#include "debug.h"
#include "memory.h"
#include "prelude.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    cleanup(chunk_free) chunk chunk = chunk_new();
    chunk_add_instruction(&chunk, OP_RETURN);
    int constant_i = chunk_add_constant(&chunk, 1.2);
    chunk_add_instruction(&chunk, OP_CONSTANT);
    chunk_add_instruction(&chunk, (uint8_t)constant_i);
    disassemble_chunk(&chunk, "test chunk");
    return 0;
}
