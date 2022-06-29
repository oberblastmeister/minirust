#include "compiler.h"
#include "ast.h"
#include "opcode.h"
#include <stdio.h>

static chunk *current_chunk(compiler *compiler) { return &compiler->chunk; }

static void emit(compiler *compiler, uint8_t byte) {
    uint8_t_vec_push(&current_chunk(compiler)->instructions, byte);
}

static void emit2(compiler *compiler, uint8_t byte1, uint8_t byte2) {
    emit(compiler, byte1);
    emit(compiler, byte2);
}

static void error(compiler *compiler, char *s) {
    compiler->did_error = true;
    fputs(s, stderr);
    fputs("\n", stderr);
}

static uint8_t add_constant(compiler *compiler, value value) {
    int constant = chunk_add_constant(current_chunk(compiler), value);
    if (constant > UINT8_MAX) {
        error(compiler, "Too many constants in one chunk.");
        return 0;
    }
    return (uint8_t)constant;
}

static void emit_constant(compiler *compiler, value value) {
    emit2(compiler, OP_CONST, add_constant(compiler, value));
}

static void todo(compiler *compiler) {
    error(compiler, "Don't know how to compile this expression yet");
}

static void compile_bin_expr(compiler *compiler, expr_bin expr_bin) {
    compile_expr(compiler, expr_bin.left);
    compile_expr(compiler, expr_bin.right);
    switch (expr_bin.op) {
    case EXPR_OP_ADD: {
        emit(compiler, OP_ADD);
        break;
    }
    default: {
        todo(compiler);
        break;
    }
    }
}

void compile_expr(compiler *compiler, expr *expr) {
    switch (expr->tag) {
    case EXPR_INT: {
        emit_constant(compiler, value_int(expr->data.expr_int));
        break;
    }
    case EXPR_DOUBLE: {
        emit_constant(compiler, value_double(expr->data.expr_double));
        break;
    }
    case EXPR_BIN: {
        compile_bin_expr(compiler, expr->data.expr_bin);
        break;
    }
    case EXPR_BOOL: {
        if (expr->data.expr_bool == true) {
            emit(compiler, OP_TRUE);
        } else {
            emit(compiler, OP_FALSE);
        }
        break;
    }
    default: {
        todo(compiler);
        break;
    }
    }
}

void compile_return(compiler *compiler, expr *expr) {
    if (expr == NULL) {
        emit(compiler, OP_RET);
    } else {
        todo(compiler);
    }
}

compiler compiler_new(void) {
    return (compiler){
        .did_error = false,
        .chunk = chunk_new(),
    };
}
