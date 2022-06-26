// based on https://stackoverflow.com/questions/48850242/thread-safe-reentrant-bison-flex

%define parse.error detailed
%define api.pure full
%locations
// parameter for both the parser and lexer
%param { yyscan_t scanner }
// parser parameter
%parse-param { parser_state *parser_state }

%code top {
#include <stdio.h>
#include <stdlib.h>
#include "lexer_state.h"
#include "lexer_wrapper.h"
} 
%code requires {
#include "parser_state.h"
#include "string_vec.h"
#include "uint8_t_vec.h"
typedef void* yyscan_t;
}
%code {
int yylex(YYSTYPE* yylvalp, YYLTYPE* yyllocp, yyscan_t scanner);
void yyerror(YYLTYPE* yyllocp, yyscan_t scanner, parser_state *parser_state, const char* msg);
}

%union {
	int int_value;
	double float_value;
	string_vec string_vec_value;
}

// values
%token<int_value> TOKEN_INT
%token<float_value> TOKEN_FLOAT
%token<string_vec_value> TOKEN_STRING
// delimiters
%token TOKEN_LBRACE TOKEN_RBRACE
%token TOKEN_LPAREN TOKEN_RPAREN
// puncutation
%token TOKEN_COMMA TOKEN_DOT
// operators
%token TOKEN_PLUS TOKEN_MINUS TOKEN_STAR TOKEN_SLASH 
%token TOKEN_BANG 
%token TOKEN_BANG_EQ TOKEN_EQ TOKEN_EQ_EQ
%token TOKEN_GT TOKEN_GT_EQ 
%token TOKEN_LT TOKEN_LT_EQ
// identifiers
%token TOKEN_IDENT
// keywords
%token TOKEN_AND TOKEN_OR
%token TOKEN_IF TOKEN_ELSE TOKEN_WHILE TOKEN_LET TOKEN_MUT
%token TOKEN_FALSE TOKEN_TRUE
%token TOKEN_FUN TOKEN_FOR TOKEN_PRINT TOKEN_RETURN 
%token TOKEN_CLASS TOKEN_SUPER TOKEN_THIS
// lexer errors
%token<string_vec_value> TOKEN_ERROR

// used during error recovery
%destructor { uint8_t_vec_free(&$$); } <string_vec_value>

// precedence
%left TOKEN_PLUS TOKEN_MINUS
%left TOKEN_STAR TOKEN_SLASH

%type<int_value> expression
%type<float_value> mixed_expression

%start calculation

%%

calculation
	: /* EMPTY */
	// | calculation line
;

// line
// :TOKEN_NEWLINE
//     | mixed_expression TOKEN_NEWLINE { printf("\tResult: %f\n", $1);}
//     | expression TOKEN_NEWLINE { printf("\tResult: %i\n", $1); }
//     | TOKEN_QUIT TOKEN_NEWLINE { printf("bye!\n"); exit(0); }
// ;

// mixed_expression
// 	: TOKEN_FLOAT { $$ = $1; }
// 	| mixed_expression TOKEN_PLUS mixed_expression	 { $$ = $1 + $3; }
// 	| mixed_expression TOKEN_MINUS mixed_expression	 { $$ = $1 - $3; }
// 	| mixed_expression TOKEN_STAR mixed_expression { $$ = $1 * $3; }
// 	| mixed_expression TOKEN_SLASH mixed_expression	 { $$ = $1 / $3; }
// 	| TOKEN_LEFT mixed_expression TOKEN_RIGHT		 { $$ = $2; }
// 	| expression TOKEN_PLUS mixed_expression	 	 { $$ = $1 + $3; }
// 	| expression TOKEN_MINUS mixed_expression	 	 { $$ = $1 - $3; }
// 	| expression TOKEN_STAR mixed_expression 	 { $$ = $1 * $3; }
// 	| expression TOKEN_SLASH mixed_expression	 { $$ = $1 / $3; }
// 	| mixed_expression TOKEN_PLUS expression	 	 { $$ = $1 + $3; }
// 	| mixed_expression TOKEN_MINUS expression	 	 { $$ = $1 - $3; }
// 	| mixed_expression TOKEN_STAR expression 	 { $$ = $1 * $3; }
// 	| mixed_expression TOKEN_SLASH expression	 { $$ = $1 / $3; }
// 	| expression TOKEN_SLASH expression		 { $$ = $1 / (float)$3; }
// ;

// expression
// 	: TOKEN_INT
// 		{
// 			$$ = $1;
// 		}
// 	| expression TOKEN_PLUS expression	{ $$ = $1 + $3; }
// 	| expression TOKEN_MINUS expression	{ $$ = $1 - $3; }
// 	| expression TOKEN_STAR expression	{ $$ = $1 * $3; }
// 	| TOKEN_LEFT expression TOKEN_RIGHT		{ $$ = $2; }
// ;

%%

void parse_file(FILE *in, parser_state *parser_state) {
	yyscan_t scanner;          
  	yylex_init_extra(parser_state, &scanner);
	yyset_in(in, scanner);
	int res = yyparse(parser_state, scanner);
	yylex_destroy(scanner);
}
	
run_calculator() {

	// yyin = stdin;

	// do {
	// 	yyparse();
	// } while(!feof(yyin));

	return 0;
}

void yyerror(YYLTYPE* yyllocp, yyscan_t scanner, parser_state *parser_state, const char *s) {
	fprintf(stderr, "%d:%d: Parse error: %s", yyllocp->first_line, yyllocp->first_column, s);
	/* char *last_error = scanner->yyextra_r.last_error;
	if (last_error[0] != '\0') {
		fprintf(stderr, ": %s\n", last_error);
	} */
	parser_state->errors_amount++;
}
