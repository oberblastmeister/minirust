#include "ast.h"
#include "memory.h"

#define VEC_TYPE expr
#define VEC_TYPE_FREE expr_free
#include "vec.h"

#define ARENA_TYPE expr
#define ARENA_VEC expr_vec
#include "arena.h"

#define VEC_TYPE stmt
#define VEC_TYPE_FREE stmt_free
#include "vec.h"

#define ARENA_TYPE stmt
#define ARENA_VEC stmt_vec
#include "arena.h"

#define VEC_TYPE decl
#include "vec.h"

#define VEC_TYPE if_cont
#define VEC_TYPE_FREE if_cont_free
#include "vec.h"

#define ARENA_TYPE if_cont
#define ARENA_VEC if_cont_vec
#include "arena.h"

ast_arena ast_arena_new(void) {
    return (ast_arena){
#define X(T) T##_arena_new(),
        ARENA_LIST_X
#undef X
    };
}

void ast_arena_free(ast_arena *ast_arena) {
#define X(T) T##_arena_free(&ast_arena->T##_arena);
    ARENA_LIST_X
#undef X
}

void expr_block_free(expr_block *block) { stmt_vec_free(&block->stmts); }

void if_cont_free(if_cont *cont) {
    switch (cont->tag) {
    case IF_CONT_ELSE: {
        expr_block_free(&cont->data.cont_else);
        break;
    }
    case IF_CONT_IF_ELSE: {
        expr_block_free(&cont->data.cont_if_else.body);
        break;
    }
    default: {
        break;
    }
    }
}

void expr_free(expr *expr) {
    switch (expr->tag) {
    case EXPR_IF: {
        expr_block_free(&expr->data.expr_if.then_expr);
        break;
    }
    case EXPR_WHILE: {
        expr_block_free(&expr->data.expr_while.body);
        break;
    }
    case EXPR_LOOP: {
        expr_block_free(&expr->data.expr_loop.body);
        break;
    }
    case EXPR_FUN: {
        expr_fun expr_fun = expr->data.expr_fun;
        string_vec_free(&expr_fun.params);
        expr_block_free(&expr_fun.body);
        break;
    }
    case EXPR_BLOCK: {
        expr_block_free(&expr->data.expr_block);
        break;
    }
    case EXPR_CALL: {
        expr_vec_free(&expr->data.expr_call.args);
        break;
    }
    case EXPR_IDENT: {
        string_free(&expr->data.expr_ident);
        break;
    }
    case EXPR_STRING: {
        // don't free the string, as ownership has been passed to the vm
        break;
    }
    default: {
        break;
    }
    }
}

void lvalue_free(lvalue *lvalue) {
    switch (lvalue->tag) {
    case LVALUE_IDENT: {
        string_free(&lvalue->lvalue_ident);
        break;
    }
    }
}

static void stmt_set_free(stmt_set *stmt_set) {
    lvalue_free(&stmt_set->lvalue);
}

void stmt_free(stmt *stmt) {
    switch (stmt->tag) {
    case STMT_LET: {
        string_free(&stmt->data.stmt_let.name);
        break;
    }
    case STMT_SET: {
        stmt_set_free(&stmt->data.stmt_set);
        break;
    }
    case STMT_FUN: {
        stmt_fun stmt_fun = stmt->data.stmt_fun;
        string_free(&stmt_fun.name);
        string_vec_free(&stmt_fun.params);
        break;
    }
    default: {
        break;
    }
    }
}

void decl_free(decl *decl) {
    switch (decl->tag) {
    case DECL_STMT: {
        stmt_free(&decl->decl_stmt);
        break;
    }
    }
}
