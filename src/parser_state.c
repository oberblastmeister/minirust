#include "parser_state.h"
#include "ast.h"
#include "lexer_state.h"

parser_state parser_state_new(void) {
    return (parser_state){
        .errors_amount = 0,
        .lexer_state = lexer_state_new(),
        .ast_arena = ast_arena_new(),
        .stmt_vec_builder = stmt_vec_new(),
        .string_vec_builder = string_vec_new(),
        .expr_vec_builder = expr_vec_new(),
    };
}

ast_arena parser_state_free(parser_state *parser_state) {
    lexer_state_free(&parser_state->lexer_state);
    // don't free the builders because the ownership has been given to the arena
    return parser_state->ast_arena;
}
