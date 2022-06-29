#ifndef _LOX_COMPILER_H
#define _LOX_COMPILER_H

#include "ast.h"
#include "chunk.h"

typedef struct {
    bool did_error;
    int stack_length;
    chunk chunk;
} compiler;

compiler compiler_new(void);

void compile_expr(compiler *compiler, expr *expr);

void compile_return(compiler *compiler, expr *expr);

#endif
