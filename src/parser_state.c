#include "parser_state.h"
#include "ast.h"
#include "lexer_state.h"

parser_state parser_state_new(void) {
    return (parser_state){
        .errors_amount = 0,
        .lexer_state = lexer_state_new(),
        .expr_arena = expr_vec_new(),
        .stmt_vec_builder = stmt_vec_new(),
        .string_vec_vec_builder = string_vec_vec_new(),
    };
}

expr_vec parser_state_free(parser_state *parser_state) {
    lexer_state_free(&parser_state->lexer_state);
    stmt_vec_free(&parser_state->stmt_vec_builder);
    string_vec_vec_free(&parser_state->string_vec_vec_builder);
    return parser_state->expr_arena;
}
