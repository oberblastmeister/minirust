#ifndef _LOX_PARSER_STATE_H
#define _LOX_PARSER_STATE_H

#include "ast.h"
#include "lexer_state.h"
#include "string_vec.h"

#define PARSER_STATE_BUILDER_LIST_X                                            \
    X(stmt)                                                                    \
    X(string)                                                                  \
    X(expr)

typedef struct {
    int errors_amount;
    lexer_state lexer_state;
    ast_arena ast_arena;
#define X(T) T##_vec T##_vec_builder;
    PARSER_STATE_BUILDER_LIST_X
#undef X
} parser_state;

parser_state parser_state_new(void);

ast_arena parser_state_free(parser_state *parser_state);

void parser_state_free_all(parser_state *parser_state);

#endif
