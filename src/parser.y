%{

#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);

%}

%define parse.error detailed

%locations

%union {
	int int_value;
	double float_value;
	char *string_value;
}

// values
%token<int_value> TOKEN_INT
%token<float_value> TOKEN_FLOAT
%token<string_value> TOKEN_STRING
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
%token TOKEN_IF TOKEN_ELSE TOKEN_WHILE TOKEN_LET
%token TOKEN_FALSE TOKEN_TRUE
%token TOKEN_FUN TOKEN_FOR TOKEN_PRINT TOKEN_RETURN 
%token TOKEN_CLASS TOKEN_SUPER TOKEN_THIS
// lexer errors
%token TOKEN_ERROR

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

run_calculator() {
	yyin = stdin;

	do {
		yyparse();
	} while(!feof(yyin));

	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "%d:%d: Parse error: %s\n", yylloc.first_line, yylloc.first_column, s);
}
