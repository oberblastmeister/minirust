#include "compiler.h"
#include "debug.h"
#include "parser_wrapper.h"
#include "vm.h"

interpret_result interpret_expr(char *s) {
    cleanup(parser_state_free_all) parser_state parser_state =
        parser_state_new();
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
