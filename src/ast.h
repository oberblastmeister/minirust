#ifndef _LOX_AST_H
#define _LOX_AST_H

#include "memory.h"
#include "string_vec.h"
#include "string_vec_vec.h"
#include "token_wrapper.h"
#include "value.h"
#include <stdlib.h>

typedef struct expr expr;

typedef struct stmt stmt;

typedef struct {
    string_vec name;
    expr *expr;
} stmt_let;

struct stmt {
    enum {
        STMT_EXPR,
        STMT_LET,
    } tag;
    union {
        expr *stmt_expr;
        stmt_let stmt_let;
    } data;
};

#define VEC_TYPE stmt
#include "vec_h.h"

typedef struct {
    stmt_vec stmts;
    expr *last;
} expr_block;

#define VEC_TYPE expr
#include "vec_h.h"

typedef struct {
    enum {
        EXPR_OP_ADD,
        EXPR_OP_SUB,
        EXPR_OP_MUL,
        EXPR_OP_DIV,
        EXPR_OP_OR,
        EXPR_OP_AND,
        EXPR_OP_LT,
        EXPR_OP_LT_EQ,
        EXPR_OP_GT,
        EXPR_OP_GT_EQ,
        EXPR_OP_EQ,
        EXPR_OP_NEQ,
    } op;
    expr *left;
    expr *right;
} expr_bin;

typedef struct {
    enum {
        EXPR_OP_NOT,
    } op;
    expr *expr;
} expr_unary;

typedef struct {
    expr *cond;
    expr_block then_expr;
} expr_if;

typedef struct {
    expr *cond;
    expr_block then_expr;
    expr_block else_expr;
} expr_if_else;

typedef struct {
    expr *cond;
    expr_block body;
} expr_while;

typedef struct {
    expr_block body;
} expr_loop;

typedef struct {
    string_vec_vec params;
    expr_block body;
} expr_fun;

typedef struct {
    expr *expr_fun;
    expr_vec args;
} expr_call;

struct expr {
    enum {
        EXPR_INT,
        EXPR_DOUBLE,
        EXPR_BOOL,
        EXPR_BIN,
        EXPR_UNARY,
        EXPR_IF,
        EXPR_IF_ELSE,
        EXPR_WHILE,
        EXPR_LOOP,
        EXPR_FUN,
        EXPR_RETURN,
        EXPR_BLOCK,
        EXPR_CALL,
        EXPR_IDENT,
    } tag;
    union {
        int expr_int;
        double expr_double;
        bool expr_bool;
        expr_bin expr_bin;
        expr_unary expr_unary;
        expr_if expr_if;
        expr_if_else expr_if_else;
        expr_while expr_while;
        expr_loop expr_loop;
        expr_fun expr_fun;
        expr *expr_return;
        expr_block expr_block;
        expr_call expr_call;
        string_vec expr_ident;
    } data;
};

typedef struct {
    enum {
        DECL_STMT,
    } tag;
    union {
        stmt decl_stmt;
    };
} decl;

#define VEC_TYPE decl
#include "vec_h.h"

typedef struct {
    decl_vec decls;
} program;
#endif
