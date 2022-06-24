%option noyywrap

%{
#include <stdio.h>
#include "token.h"

#define YY_DECL enum yytokentype yylex()

static int lexer_column = 1;

static int lexer_line = 1;

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = lexer_line; \
    yylloc.first_column = lexer_column; \
    yylloc.last_column = lexer_column + yyleng - 1; \
    lexer_column += yyleng;
%}

DIGIT [0-9]

%%

[ \t\r] ; // ignore all whitespace
{DIGIT}+"."{DIGIT}+ {
    yylval.fval = atof(yytext);
    return T_FLOAT;
}
{DIGIT}+ {
    yylval.ival = atoi(yytext);
    return T_INT;
}
\n {
    lexer_column = 1;
    lexer_line++;
    return T_NEWLINE;
}
"+" {
    return T_PLUS; 
}
"-" {
    return T_MINUS; 
}
"*" {
    return T_MULTIPLY; 
}
"/" {
    return T_DIVIDE; 
}
"(" {
    return T_LEFT; 
}
")" {
    return T_RIGHT; 
}
"exit" {
    return T_QUIT; 
}
"quit" {
    return T_QUIT; 
}
. {
    return T_ERROR;
}

%%
