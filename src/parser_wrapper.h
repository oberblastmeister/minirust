#ifndef _PARSER_WRAPPER_H
#define _PARSER_WRAPPER_H

#include "token.h"

yyparse_expr_t parse_string_expr(char *s, parser_state *parser_state);

#endif
