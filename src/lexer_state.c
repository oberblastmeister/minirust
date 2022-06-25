#include "lexer_state.h"

lexer_state lexer_state_new(void) {
    return (lexer_state){.lexer_column = 1,
                         .lexer_line = 1,
                         .string_builder = uint8_t_vec_new()};
}

void lexer_state_reset(lexer_state *state) {
    state->lexer_column = 1;
    state->lexer_line = 1;
}

void lexer_state_free(lexer_state *state) {
    uint8_t_vec_free(&state->string_builder);
}
