#ifndef _LOX_PARSER_STATE_H
#define _LOX_PARSER_STATE_H

#include "lexer_state.h"
#include "string_vec_vec.h"
#include "ast.h"

typedef struct {
    int errors_amount;
    lexer_state lexer_state;
    expr_vec expr_arena;
    stmt_vec stmt_vec_builder;
    string_vec_vec string_vec_vec_builder;
} parser_state;

parser_state parser_state_new(void);

expr_vec parser_state_free(parser_state *parser_state);

#endif
