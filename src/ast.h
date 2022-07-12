#ifndef _LOX_AST_H
#define _LOX_AST_H

#include "char_vec.h"
#include "memory.h"
#include "string_vec.h"
#include "token_wrapper.h"
#include "value.h"
#include <stdlib.h>

typedef struct expr expr;

typedef struct stmt stmt;

typedef struct {
    string name;
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

#define ARENA_TYPE stmt
#define ARENA_VEC stmt_vec
#include "arena_h.h"

typedef struct {
    stmt_vec stmts;
    // can be null
    expr *last;
} expr_block;

#define VEC_TYPE expr
#include "vec_h.h"

#define ARENA_TYPE expr
#define ARENA_VEC expr_vec
#include "arena_h.h"

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

typedef struct if_cont if_cont;

typedef struct {
    expr *cond;
    expr_block body;
    if_cont *cont;
} if_cont_if_else;

typedef struct {
} cont_none;

struct if_cont {
    enum {
        IF_CONT_IF_ELSE,
        IF_CONT_ELSE,
        IF_CONT_NONE,
    } tag;
    struct {
        expr_block cont_else;
        if_cont_if_else cont_if_else;
        cont_none cont_none;
    } data;
};

#define VEC_TYPE if_cont
#include "vec_h.h"

#define ARENA_TYPE if_cont
#define ARENA_VEC if_cont_vec
#include "arena_h.h"

typedef struct {
    expr *cond;
    expr_block body;
} expr_while;

typedef struct {
    expr *cond;
    expr_block then_expr;
    if_cont *cont;
} expr_if;

typedef struct {
    expr_block body;
} expr_loop;

typedef struct {
    string_vec params;
    expr_block body;
} expr_fun;

typedef struct {
    expr *expr_fun;
    expr_vec args;
} expr_call;

typedef struct {
} expr_nil;
struct expr {
    enum {
        EXPR_NIL,
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
        expr_nil expr_nil;
        int expr_int;
        double expr_double;
        bool expr_bool;
        expr_bin expr_bin;
        expr_unary expr_unary;
        expr_if expr_if;
        expr_while expr_while;
        expr_loop expr_loop;
        expr_fun expr_fun;
        expr *expr_return;
        expr_block expr_block;
        expr_call expr_call;
        string expr_ident;
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

#define AST_MANAGED_LIST_X                                                     \
    X(expr)                                                                    \
    X(if_cont)                                                                 \
    X(stmt)

typedef struct {
    expr_arena expr_arena;
    if_cont_arena if_cont_arena;
} ast_arena;

ast_arena ast_arena_new(void);

void ast_arena_free(ast_arena *ast_arena);

void stmt_free(stmt *stmt);

void if_cont_free(if_cont *cont);

void expr_free(expr *expr);

#endif
