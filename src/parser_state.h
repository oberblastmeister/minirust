#ifndef _LOX_PARSER_STATE_H
#define _LOX_PARSER_STATE_H

#include "ast.h"
#include "lexer_state.h"
#include "string_vec.h"

typedef struct {
    int errors_amount;
    lexer_state lexer_state;
    ast_arena ast_arena;
} parser_state;

parser_state parser_state_new(void);

void parser_state_free(parser_state *parser_state);

#endif
