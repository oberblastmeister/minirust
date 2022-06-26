#include "parser_state.h"
#include "lexer_state.h"

parser_state parser_state_new(void) {
    return (parser_state){.errors_amount = 0, .lexer_state = lexer_state_new()};
}
