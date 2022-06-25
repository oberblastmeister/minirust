%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "uint8_t_vec.h"
#include "vec_util.h"
#include "lexer_state.h"

// #define YY_DECL enum yytokentype yylex()

// #define YY_DECL enum yytokentype yylex(YYSTYPE *yylval, YYLTYPE *yylloc, lexer_state *state)

// static int lexer_column = 1;

// static int lexer_line = 1;

// static uint8_t_vec string_builder = const_vec_new();

#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line = yyextra->lexer_line; \
    yylloc->first_column = yyextra->lexer_column; \
    yylloc->last_column = yyextra->lexer_column + yyleng - 1; \
    yyextra->lexer_column += yyleng;
%}

%option reentrant
%option bison-bridge bison-locations
%option extra-type="lexer_state *"

%x STRING_SC

DIGIT [0-9]
ALPHA [a-zA-Z]

%%

[ \t\r] ; // ignore all whitespace
{DIGIT}+"."{DIGIT}+ {
    yylval->float_value = strtod(yytext, NULL);
    return TOKEN_FLOAT;
}
{DIGIT}+ {
    yylval->int_value = atoi(yytext);
    return TOKEN_INT;
}
\n {
    yyextra->lexer_column = 1;
    yyextra->lexer_line++;
}
"+" {
    return TOKEN_PLUS; 
}
"-" {
    return TOKEN_MINUS; 
}
"*" {
    return TOKEN_STAR; 
}
"/" {
    return TOKEN_SLASH; 
}
"(" {
    return TOKEN_LPAREN; 
}
")" {
    return TOKEN_RPAREN; 
}
\" {
    uint8_t_vec_clear(&yyextra->string_builder);
    BEGIN(STRING_SC);
}
<STRING_SC>\" {
    BEGIN(0);
}
<STRING_SC>. {
    // only supports ascii, so only one byte
    uint8_t_vec_push(&yyextra->string_builder, yytext[0]);
}
. {
    return TOKEN_ERROR;
}

%%

yyscan_t new_scanner(FILE *in, lexer_state *lexer_state) {
    yyscan_t scanner;
    yylex_init_extra(lexer_state, &scanner);
    yyset_in(in, scanner);
    return scanner;
}
