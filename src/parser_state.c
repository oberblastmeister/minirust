#include "parser_state.h"
#include "ast.h"
#include "lexer_state.h"

parser_state parser_state_new(void) {
    return (parser_state){0, lexer_state_new(), ast_arena_new(),
#define X(T) T##_vec_new(),
                          PARSER_STATE_BUILDER_LIST_X
#undef X
    };
}

ast_arena parser_state_free(parser_state *parser_state) {
    lexer_state_free(&parser_state->lexer_state);
    // don't free the builders because the ownership has been given to the arena
    return parser_state->ast_arena;
}

void parser_state_free_all(parser_state *parser_state) {
    ast_arena arena = parser_state_free(parser_state);
    ast_arena_free(&arena);
}
