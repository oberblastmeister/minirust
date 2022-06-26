#ifndef _LOX_PARSER_STATE_H
#define _LOX_PARSER_STATE_H

#include "lexer_state.h"

typedef struct {
    int errors_amount;
    lexer_state lexer_state;
} parser_state;

parser_state parser_state_new(void);

void parse_state_free(parser_state *parser_state);

#endif
