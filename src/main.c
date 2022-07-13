#include "chunk.h"
#include "compiler.h"
#include "debug.h"
#include "io_ext.h"
#include "lexer_wrapper.h"
#include "memory.h"
#include "opcode.h"
#include "parser.h"
#include "parser_wrapper.h"
#include "prelude.h"
#include "uint8_t_vec.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interpret_result interpret(char *s) {
    parser_state parser_state = parser_state_new();
    yyparse_expr_t res = parse_string_expr(s, &parser_state);
    if (res.yynerrs > 0) {
        return INTERPRET_PARSE_ERROR;
    }
    expr expr = res.yyvalue;
    cleanup(compiler_free) compiler compiler = compiler_new();
    compile_expr(&compiler, &expr);
    compile_return(&compiler, NULL);
    disassemble_chunk(&compiler.chunk, "repl");
    if (compiler.did_error) {
        return INTERPRET_COMPILE_ERROR;
    }
    cleanup(vm_free) vm vm = vm_new(compiler.chunk, compiler.objects);
    return vm_run(&vm);
}

static void repl() {
    char line[1024];
    while (true) {
        put("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        interpret(line);
    }
}

// static void run_file(const char *path){
//     char *source = read
// }

int main(int argc, const char *argv[]) {
    if (getenv("LOX_PARSER_DEBUG") != NULL) {
        extern int yydebug;
        yydebug = 1;
    }
    // run_lexer_debug();
    // interpret("2.0 + 1.0");
    repl();

    // chunk chunk = chunk_new();
    // int constant_i = chunk_add_constant(&chunk, 1.2);
    // chunk_add_instruction(&chunk, OP_CONSTANT);
    // chunk_add_instruction(&chunk, constant_i);
    // int constant_i2 = chunk_add_constant(&chunk, 123);
    // chunk_add_instruction(&chunk, OP_CONSTANT);
    // chunk_add_instruction(&chunk, constant_i2);
    // chunk_add_instruction(&chunk, OP_NEGATE);
    // int constant_i3 = chunk_add_constant(&chunk, 5.6);
    // chunk_add_instruction(&chunk, OP_CONSTANT);
    // chunk_add_instruction(&chunk, constant_i3);
    // int constant_i4 = chunk_add_constant(&chunk, 1.2);
    // chunk_add_instruction(&chunk, OP_CONSTANT);
    // chunk_add_instruction(&chunk, constant_i4);
    // chunk_add_instruction(&chunk, OP_DIVIDE);
    // chunk_add_instruction(&chunk, OP_RETURN);
    // disassemble_chunk(&chunk, "test chunk");
    // cleanup(vm_free) vm vm = vm_new(chunk);
    // vm_run(&vm);

    return 0;
}

// void test(int n, char s[n]) {

// }
