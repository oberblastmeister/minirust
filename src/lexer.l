%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "uint8_t_vec.h"
#include "vec_util.h"

#define YY_DECL enum yytokentype yylex()

static int lexer_column = 1;

static int lexer_line = 1;

static uint8_t_vec string_builder = const_vec_new();

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = lexer_line; \
    yylloc.first_column = lexer_column; \
    yylloc.last_column = lexer_column + yyleng - 1; \
    lexer_column += yyleng;
%}

// exclusive start condition
// when BEGIN, only rules with STRING_SC will be considered
%x STRING_SC

DIGIT [0-9]
ALPHA [a-zA-Z]

%%

[ \t\r] ; // ignore all whitespace
{DIGIT}+"."{DIGIT}+ {
    yylval.float_value = strtod(yytext, NULL);
    return TOKEN_FLOAT;
}
{DIGIT}+ {
    yylval.int_value = atoi(yytext);
    return TOKEN_INT;
}
\n {
    lexer_column = 1;
    lexer_line++;
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
    uint8_t_vec_clear(&string_builder);
    BEGIN(STRING_SC);
}
<STRING_SC>\" {
    BEGIN(0);
}
<STRING_SC>. {
    // only supports ascii, so only one byte
    uint8_t_vec_push(&string_builder, yytext[0]);
}
. {
    return TOKEN_ERROR;
}

%%
