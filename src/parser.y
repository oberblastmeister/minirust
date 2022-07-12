// based on https://stackoverflow.com/questions/48850242/thread-safe-reentrant-bison-flex

%define parse.trace
%define parse.error detailed
%define api.pure full
%locations
// parameter for both the parser and lexer
%param { yyscan_t scanner }
// parser parameter
%parse-param { parser_state *ps }

%code top {
#include <stdio.h>
#include <stdlib.h>
#include "lexer_wrapper.h"
#include "parser_util.h"
} 
%code requires {
#include "char_vec.h"
#include "string_vec.h"
#include "ast.h"
#include "uint8_t_vec.h"
#include "lexer_state.h"
#include "parser_state.h"

typedef void* yyscan_t;

}
%code {
int yylex(YYSTYPE* yylvalp, YYLTYPE* yyllocp, yyscan_t scanner);
void yyerror(YYLTYPE* yyllocp, yyscan_t scanner, parser_state *ps, const char* msg);
}

%union {
	int int_value;
	double double_value;
	string string_value;
	string_vec string_vec_value;
	expr expr;
	stmt stmt;
	decl decl;
	expr_block expr_block;
	expr_vec expr_vec;
	if_cont if_cont;
	value value;
}

// values
%token TOKEN_NIL
%token<int_value> TOKEN_INT
%token<double_value> TOKEN_DOUBLE
%token<string_value> TOKEN_STRING
// delimiters
%token TOKEN_LBRACE TOKEN_RBRACE
%token TOKEN_LPAREN TOKEN_RPAREN
// puncutation
%token TOKEN_COMMA TOKEN_DOT TOKEN_SEMI
// operators
%token TOKEN_PLUS TOKEN_MINUS TOKEN_STAR TOKEN_SLASH 
%token TOKEN_BANG 
%token TOKEN_BANG_EQ TOKEN_EQ TOKEN_EQ_EQ
%token TOKEN_GT TOKEN_GT_EQ 
%token TOKEN_LT TOKEN_LT_EQ
// identifiers
%token<string_value> TOKEN_IDENT
// keywords
%token TOKEN_AND TOKEN_OR
%token TOKEN_IF TOKEN_ELSE TOKEN_WHILE TOKEN_LET TOKEN_MUT
%token TOKEN_FALSE TOKEN_TRUE
%token TOKEN_FUN TOKEN_FOR TOKEN_PRINT TOKEN_RETURN TOKEN_LOOP
%token TOKEN_CLASS TOKEN_SUPER TOKEN_THIS
// lexer errors
%token TOKEN_ERROR

// used during error recovery
%destructor { string_free(&$$); } <string_value>

// precedence
%left TOKEN_RETURN
%nonassoc TOKEN_EQ_EQ TOKEN_BANG_EQ
%nonassoc TOKEN_GT TOKEN_GT_EQ
%nonassoc TOKEN_LT TOKEN_LT_EQ
%left TOKEN_OR
%left TOKEN_AND
%left TOKEN_PLUS TOKEN_MINUS
%left TOKEN_STAR TOKEN_SLASH
%left TOKEN_BANG

%type<decl> decl
%type<stmt> stmt
%type<expr> expr expr_op expr_atom expr_fun expr_call lit
%type<expr> expr_if expr_control_arg expr_no_control_arg expr_block_lift
%type<expr> expr_atom_no_block expr_op_no_block expr_no_block expr_call_no_block
%type<if_cont> if_cont
%type<expr_block> expr_block
%type<string_vec_value> params
%type<expr_vec> args

%start decl stmt expr expr_atom expr_block
%start expr_fun nothing

%%

decl
	: stmt { $$ = (decl){ DECL_STMT, { .decl_stmt = $1 } }; }
;
	 
stmt
	: expr_no_block TOKEN_SEMI { $$ = (stmt){ STMT_EXPR, { .stmt_expr = ALLOC_EXPR($1) } }; }
	| expr_block_lift { $$ = (stmt){ STMT_EXPR, { .stmt_expr = ALLOC_EXPR($1) } }; }
	| stmt TOKEN_SEMI { $$ = $1; }
	| TOKEN_LET TOKEN_IDENT TOKEN_EQ expr TOKEN_SEMI
		{
			$$ = (stmt) {
				STMT_LET,
				{
					.stmt_let = {
						.name = $2,
						.expr = ALLOC_EXPR($4),
					}
				}
			};
		}
;

nothing
	: %empty { }

expr_no_block
	: expr_op_no_block { $$ = $1; }
	| expr_fun { $$ = $1; }
	| TOKEN_RETURN expr_op { $$ = (expr){ EXPR_RETURN, { .expr_return = ALLOC_EXPR($2) } }; }
	| TOKEN_RETURN { $$ = (expr){EXPR_RETURN, { .expr_return = NULL }}; }

expr
	: expr_control_arg { $$ = $1; }
	| expr_no_control_arg { $$ = $1; }
;

expr_no_control_arg
	: expr_op { $$ = $1; }
	| expr_fun { $$ = $1; }
	| expr_if { $$ = $1; }
	| TOKEN_RETURN { $$ = (expr){EXPR_RETURN, { .expr_return = NULL }}; }
;


expr_control_arg
	: TOKEN_RETURN expr_op { $$ = (expr){ EXPR_RETURN, { .expr_return = ALLOC_EXPR($2) } }; }
;

// these need expr_no_control_arg because the control expression can be by themselves,
// or they can take an argument. So the block the comes right after can be ambiguous.
// is it an argument? or is it part of the if statement?
expr_if
	: TOKEN_IF expr_no_control_arg expr_block if_cont
		{
			$$ = (expr){
				EXPR_IF,
				{
					.expr_if = {
						.cond = ALLOC_EXPR($2),
						.then_expr = $3,
						.cont = if_cont_arena_alloc(&ps->ast_arena.if_cont_arena, $4),
					}
				}
			};
		}
;

if_cont
	: %empty { $$ = (if_cont){IF_CONT_NONE, {.cont_none = { } }}; }
	| TOKEN_ELSE expr_block { $$ = (if_cont){IF_CONT_ELSE, {.cont_else = $2} }; }
	| TOKEN_ELSE TOKEN_IF expr_no_control_arg expr_block if_cont	
		{
			$$ = (if_cont){
				IF_CONT_IF_ELSE,
				{
					.cont_if_else = {
						.cond = ALLOC_EXPR($3),
						.body = $4,
						.cont = if_cont_arena_alloc(&ps->ast_arena.if_cont_arena, $5),
					}
				}
			};
		}
;

expr_fun
	: TOKEN_FUN TOKEN_LPAREN params TOKEN_RPAREN expr_block
		{
			$$ = (expr){
				EXPR_FUN,
				{
					.expr_fun = {
						.params = $3,
						.body = $5,
					}
				}
			};
		}
;

params
	: %empty { }
	| params_build maybe_comma { $$ = string_vec_take(&ps->string_vec_builder); }
;

params_build
	: TOKEN_IDENT { string_vec_push(&ps->string_vec_builder, $1); }
	| params_build TOKEN_COMMA TOKEN_IDENT { string_vec_push(&ps->string_vec_builder, $3); }
;

expr_op_no_block
	: expr_op_no_block TOKEN_PLUS expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_ADD, $3); }
	| expr_op_no_block TOKEN_MINUS expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_SUB, $3); }
	| expr_op_no_block TOKEN_STAR expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_MUL, $3); }
	| expr_op_no_block TOKEN_SLASH expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_DIV, $3); }
	| expr_op_no_block TOKEN_AND expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_AND, $3); }
	| expr_op_no_block TOKEN_OR expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_OR, $3); }
	| expr_op_no_block TOKEN_GT expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_GT, $3); }
	| expr_op_no_block TOKEN_GT_EQ expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_GT_EQ, $3); }
	| expr_op_no_block TOKEN_LT_EQ expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_LT_EQ, $3); }
	| expr_op_no_block TOKEN_EQ_EQ expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_EQ, $3); }
	| expr_op_no_block TOKEN_BANG_EQ expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_NEQ, $3); }
	| TOKEN_BANG expr_op { $$ = MAKE_EXPR_UNARY(EXPR_OP_NOT, $2); }
	| expr_atom_no_block { $$ = $1; }
	
;	

expr_op
	: expr_op TOKEN_PLUS expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_ADD, $3); }
	| expr_op TOKEN_MINUS expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_SUB, $3); }
	| expr_op TOKEN_STAR expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_MUL, $3); }
	| expr_op TOKEN_SLASH expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_DIV, $3); }
	| expr_op TOKEN_AND expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_AND, $3); }
	| expr_op TOKEN_OR expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_OR, $3); }
	| expr_op TOKEN_GT expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_GT, $3); }
	| expr_op TOKEN_GT_EQ expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_GT_EQ, $3); }
	| expr_op TOKEN_LT_EQ expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_LT_EQ, $3); }
	| expr_op TOKEN_EQ_EQ expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_EQ, $3); }
	| expr_op TOKEN_BANG_EQ expr_op { $$ = MAKE_EXPR_BIN($1, EXPR_OP_NEQ, $3); }
	| TOKEN_BANG expr_op { $$ = MAKE_EXPR_UNARY(EXPR_OP_NOT, $2); }
	| expr_atom { $$ = $1; }
	
;	

expr_block_lift
	: expr_block { $$ = (expr) {EXPR_BLOCK, {.expr_block = $1}}; }
; 

expr_block
	: TOKEN_LBRACE stmt_build TOKEN_RBRACE
		{
			$$ = (expr_block) {
				.stmts = stmt_vec_take(&ps->stmt_vec_builder),
				.last = NULL,
			};
		}
	// must use expr_no_block here to make sure there is no reduce reduce conflict
	// TOKEN_NIL . TOKEN_LPAREN
	// don't know whether to reduce TOKEN_NIL to expr or expr_no_block
	// this depends on whether there is a semicolon in the future, but we don't know that
	// we can only have one lookahead
	| TOKEN_LBRACE stmt_build expr_no_block TOKEN_RBRACE
		{
			$$ = (expr_block) {
				.stmts = stmt_vec_take(&ps->stmt_vec_builder),
				.last = ALLOC_EXPR($3),
			};
		}
;
	
stmt_build
	: %empty { }
	| stmt_build stmt { stmt_vec_push(&ps->stmt_vec_builder, $2); }
;

expr_atom_no_block
	: lit
	| TOKEN_LPAREN expr TOKEN_RPAREN { $$ = $2; }
	| expr_call_no_block { $$ = $1; }
	| TOKEN_IDENT { $$ = (expr){EXPR_IDENT, {.expr_ident = $1}}; }
;

expr_atom
	: lit
	| TOKEN_LPAREN expr TOKEN_RPAREN { $$ = $2; }
	| expr_call { $$ = $1; }
	| expr_block { $$ = (expr){EXPR_BLOCK, { .expr_block = $1 } }; }
	| TOKEN_IDENT { $$ = (expr){EXPR_IDENT, {.expr_ident = $1}}; }
;

lit
	: TOKEN_NIL { $$ = (expr){EXPR_NIL, {.expr_nil = {}}}; }
	| TOKEN_INT { $$ = (expr){EXPR_INT, {.expr_int = $1}}; }
	| TOKEN_DOUBLE { $$ = (expr){EXPR_DOUBLE, {.expr_double = $1}}; }
	| TOKEN_TRUE { $$ = (expr){ EXPR_BOOL, {.expr_bool = true}}; }
	| TOKEN_FALSE { $$ = (expr){ EXPR_BOOL, {.expr_bool = false}}; }
	| TOKEN_STRING { $$ = (expr){EXPR_STRING, {.expr_string = $1}}; }

expr_call
	: expr_atom TOKEN_LPAREN args TOKEN_RPAREN
		{
			$$ = (expr){
				EXPR_CALL,
				{
					.expr_call = {
						.expr_fun = ALLOC_EXPR($1),
						.args = $3,
					}
				}
			};
		}
;

expr_call_no_block
	: expr_atom_no_block TOKEN_LPAREN args TOKEN_RPAREN
		{
			$$ = (expr){
				EXPR_CALL,
				{
					.expr_call = {
						.expr_fun = ALLOC_EXPR($1),
						.args = $3,
					}
				}
			};
		}
;
	
args
	: %empty { $$ = expr_vec_new(); }
	| args_build maybe_comma { $$ = expr_vec_take(&ps->expr_vec_builder); }
;

args_build
	: expr { expr_vec_push(&ps->expr_vec_builder, $1); }
	| args_build TOKEN_COMMA expr { expr_vec_push(&ps->expr_vec_builder, $3); }
;

maybe_comma
	: %empty { }
	| TOKEN_COMMA { }

%%

void parse_file(FILE *in, parser_state *ps) {
	yyscan_t scanner;          
  	yylex_init_extra(ps, &scanner);
	yyset_in(in, scanner);
	int res = yyparse(ps, scanner);
	yylex_destroy(scanner);
}

yyparse_expr_t parse_string_expr(const char *s, parser_state *ps) {
	yyscan_t scanner;          
  	yylex_init_extra(ps, &scanner);
	YY_BUFFER_STATE buf = yy_scan_string(s, scanner);
	yyparse_expr_t res = yyparse_expr(scanner, ps);
	yy_delete_buffer(buf, scanner);
	yylex_destroy(scanner);
	return res;
}
	
void yyerror(YYLTYPE* yyllocp, __attribute__((unused)) yyscan_t scanner, parser_state *ps, const char *s) {
	// don't print too many errors
	if (ps->errors_amount > 4) {
		return;
	}
	fprintf(stderr, "%d:%d: Parse error: %s", yyllocp->first_line, yyllocp->first_column, s);
	char *last_error = ps->lexer_state.last_error.data;
	if (last_error[0] != '\0') {
		fprintf(stderr, ": %s\n", last_error);
	} else {
		puts("");
	}
	ps->errors_amount++;
}
