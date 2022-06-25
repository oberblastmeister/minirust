#include "lexer_state.h"
#include "parser_state.h"
#include <stdio.h>

int run_calculator(void);

void parse_file(FILE *in, lexer_state *lexer_state, parser_state *parser_state);
