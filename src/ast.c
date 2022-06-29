#include "ast.h"
#include "memory.h"

#define VEC_TYPE expr
#include "vec.h"

#define VEC_TYPE stmt
#include "vec.h"

#define VEC_TYPE decl
#include "vec.h"

#define VEC_TYPE if_cont
#include "vec.h"

ast_arena ast_arena_new(void) {
    return (ast_arena){
        .expr_arena = expr_vec_new(),
        .if_cont_arena = if_cont_vec_new(),
    };
}

void ast_arena_free(ast_arena *ast_arena) {
    expr_vec_free(&ast_arena->expr_arena);
    if_cont_vec_free(&ast_arena->if_cont_arena);
}
