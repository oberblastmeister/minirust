#include "lexer_state.h"
#include "char_vec.h"

lexer_state lexer_state_new(void) {
    return (lexer_state){
        .lexer_column = 1,
        .lexer_line = 1,
        .string_builder = const_string_new(""),
        .last_error = const_string_new(""),
    };
}

void lexer_state_reset(lexer_state *state) {
    state->lexer_column = 1;
    state->lexer_line = 1;
    state->last_error = const_string_new("");
}

void lexer_state_free(lexer_state *state) {
    string_free(&state->string_builder);
    string_free(&state->last_error);
}
