#include "parser_state.h"
#include "ast.h"
#include "lexer_state.h"

parser_state parser_state_new(void) {
    return (parser_state){
        0,
        lexer_state_new(),
        ast_arena_new(),
    };
}

/**
 * Caution! This will free the arena that holds the ast.
 * If the parser state is freed, the ast cannot be used anymore
 */
void parser_state_free(parser_state *parser_state) {
    lexer_state_free(&parser_state->lexer_state);
    ast_arena_free(&parser_state->ast_arena);
}
