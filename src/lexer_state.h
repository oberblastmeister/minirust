#ifndef _LOX_LEXER_STATE_H
#define _LOX_LEXER_STATE_H

#include "uint8_t_vec.h"

typedef struct {
    int lexer_column;
    int lexer_line;
    uint8_t_vec string_builder;
} lexer_state;

lexer_state lexer_state_new(void);

void lexer_state_reset(lexer_state *state);

void lexer_state_free(lexer_state *state);

#endif
