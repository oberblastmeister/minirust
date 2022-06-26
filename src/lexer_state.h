#ifndef _LOX_LEXER_STATE_H
#define _LOX_LEXER_STATE_H

#include "string_vec.h"
#include "uint8_t_vec.h"

typedef struct {
    int lexer_column;
    int lexer_line;
    string_vec string_builder;
    string_vec last_error;
} lexer_state;

lexer_state lexer_state_new(void);

void lexer_state_reset(lexer_state *state);

void lexer_state_free(lexer_state *state);

#endif
