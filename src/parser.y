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
	int ival;
	float fval;
}

%token<ival> T_INT
%token<fval> T_FLOAT
%token T_PLUS T_MINUS T_MULTIPLY T_DIVIDE T_LEFT T_RIGHT
%token T_NEWLINE T_QUIT
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE
%token T_ERROR

%type<ival> expression
%type<fval> mixed_expression

%start calculation

%%

calculation
	: /* EMPTY */
	| calculation line
;

line
	: T_NEWLINE
    | mixed_expression T_NEWLINE { printf("\tResult: %f\n", $1);}
    | expression T_NEWLINE { printf("\tResult: %i\n", $1); }
    | T_QUIT T_NEWLINE { printf("bye!\n"); exit(0); }
;

mixed_expression
	: T_FLOAT { $$ = $1; }
	| mixed_expression T_PLUS mixed_expression	 { $$ = $1 + $3; }
	| mixed_expression T_MINUS mixed_expression	 { $$ = $1 - $3; }
	| mixed_expression T_MULTIPLY mixed_expression { $$ = $1 * $3; }
	| mixed_expression T_DIVIDE mixed_expression	 { $$ = $1 / $3; }
	| T_LEFT mixed_expression T_RIGHT		 { $$ = $2; }
	| expression T_PLUS mixed_expression	 	 { $$ = $1 + $3; }
	| expression T_MINUS mixed_expression	 	 { $$ = $1 - $3; }
	| expression T_MULTIPLY mixed_expression 	 { $$ = $1 * $3; }
	| expression T_DIVIDE mixed_expression	 { $$ = $1 / $3; }
	| mixed_expression T_PLUS expression	 	 { $$ = $1 + $3; }
	| mixed_expression T_MINUS expression	 	 { $$ = $1 - $3; }
	| mixed_expression T_MULTIPLY expression 	 { $$ = $1 * $3; }
	| mixed_expression T_DIVIDE expression	 { $$ = $1 / $3; }
	| expression T_DIVIDE expression		 { $$ = $1 / (float)$3; }
;

expression
	: T_INT
		{
			$$ = $1;
		}
	| expression T_PLUS expression	{ $$ = $1 + $3; }
	| expression T_MINUS expression	{ $$ = $1 - $3; }
	| expression T_MULTIPLY expression	{ $$ = $1 * $3; }
	| T_LEFT expression T_RIGHT		{ $$ = $2; }
;

%%

int run_calculator() {
	yyin = stdin;

	do {
		yyparse();
	} while(!feof(yyin));

	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "%d:%d: Parse error: %s\n", yylloc.first_line, yylloc.first_column, s);
}
