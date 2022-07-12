%{
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "char_vec.h"
#include "vec_util.h"
#include "lexer_state.h"
#include "parser_state.h"
#include "memory.h"

#define PUSH_CHAR(b) string_push(&ls.string_builder, b)

#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line = ls.lexer_line; \
    yylloc->first_column = ls.lexer_column; \
    yylloc->last_column = ls.lexer_column + yyleng - 1; \
    ls.lexer_column += yyleng;

#define ls yyextra->lexer_state
%}

%option reentrant
%option noyywrap
%option noinput nounput
%option 8bit nodefault
%option bison-bridge bison-locations
%option extra-type="parser_state *"

%x STRING_SC

DIGIT [0-9]
ALPHA [a-zA-Z]
IDENT_START {ALPHA}|"_"

%%

[ \t\r] { } // ignore all whitespace
\n {
    ls.lexer_column = 1;
    ls.lexer_line++;
}
"{" { return TOKEN_LBRACE; }
"}" { return TOKEN_RBRACE; }
"(" { return TOKEN_LPAREN; }
")" { return TOKEN_RPAREN; }
";" { return TOKEN_SEMI; }
"," { return TOKEN_COMMA; }
"." { return TOKEN_DOT; }
"+" { return TOKEN_PLUS; }
"-" { return TOKEN_MINUS; }
"*" { return TOKEN_STAR; }
"/" { return TOKEN_SLASH; }
"!" { return TOKEN_BANG; }
"!=" { return TOKEN_BANG_EQ; }
"=" { return TOKEN_EQ; }
"==" { return TOKEN_EQ_EQ; }
">" { return TOKEN_GT; }
">=" { return TOKEN_GT_EQ; }
"<" { return TOKEN_LT; }
"<=" { return TOKEN_LT_EQ; }
\" {
    string_clear(&ls.string_builder);
    BEGIN(STRING_SC);
}
<STRING_SC>\" {
    yylval->string_value = string_copy(&ls.string_builder);
    BEGIN(INITIAL);
    return TOKEN_STRING;
}
<STRING_SC>. {
    // flex only supports ascii, so . should match one byte
    PUSH_CHAR(yytext[0]);
}
<STRING_SC>\\t { PUSH_CHAR('\t'); }
<STRING_SC>\\r { PUSH_CHAR('\r'); }
<STRING_SC>\\\\ { PUSH_CHAR('\\'); }
<STRING_SC>\\n { PUSH_CHAR('\n'); }
<STRING_SC>\\\" { PUSH_CHAR('\"'); }
<STRING_SC>\\. { 
    ls.last_error = const_string_new("Invalid escape sequence");
    BEGIN(INITIAL);
    return TOKEN_ERROR;
}
<STRING_SC>\n {
    ls.last_error = const_string_new("Invalid newline in string");
    BEGIN(INITIAL);
    return TOKEN_ERROR;
}
nil { return TOKEN_NIL; }
and { return TOKEN_AND; }
or { return TOKEN_OR; }
if { return TOKEN_IF; }
else { return TOKEN_ELSE; }
while { return TOKEN_WHILE; }
loop { return TOKEN_LOOP; }
let { return TOKEN_LET; }
mut { return TOKEN_MUT; }
false { return TOKEN_FALSE; }
true { return TOKEN_TRUE; }
fun { return TOKEN_FUN; }
for { return TOKEN_FOR; }
print { return TOKEN_PRINT; }
return { return TOKEN_RETURN; }
class { return TOKEN_CLASS; }
super { return TOKEN_SUPER; }
this { return TOKEN_THIS; }
{DIGIT}+"."{DIGIT}+ {
    yylval->double_value = strtod(yytext, NULL);
    return TOKEN_DOUBLE;
}
{DIGIT}+ {
    yylval->int_value = atoi(yytext);
    return TOKEN_INT;
}
{IDENT_START}({IDENT_START}|{DIGIT})* { 
    // yyleng does not include the null
    yylval->string_value = string_new_n(yytext, yyleng + 1);
    return TOKEN_IDENT;
}
. {
    ls.last_error = const_string_new("Unknown token");
    return TOKEN_ERROR;
}

%%

void run_lexer_debug(void) {
	yyscan_t scanner;
    YYSTYPE lval;
    YYLTYPE lloc;
    cleanup(parser_state_free) parser_state parser_state = parser_state_new();
  	yylex_init_extra(&parser_state, &scanner);
	yyset_in(stdin, scanner);
    yyset_out(stdout, scanner);
    do {
        int token = yylex(&lval, &lloc, scanner);
        printf("token: %d\n", token);
    } while (true);
	yylex_destroy(scanner);
}
