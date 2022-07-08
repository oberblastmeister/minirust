#include "compiler.h"
#include "ast.h"
#include "opcode.h"
#include <stdio.h>
#include <string.h>

scope scope_copy(scope *old_scope) {
    local *anon_locals = malloc(sizeof(local) * old_scope->next_anon_id);
    memcpy(anon_locals, old_scope->anon_locals,
           sizeof(local) * old_scope->next_anon_id);
    return (scope){
        .next_anon_id = old_scope->next_anon_id,
        .anon_locals = anon_locals,
        .named_locals = local_map_copy(&old_scope->named_locals),
    };
}

void scope_free(scope *scope) {
    free(scope->anon_locals);
    local_map_free(&scope->named_locals);
}

#define HM_KEY string
#define HM_KEY_COPY string_copy
#define HM_KEY_FREE string_free
#define HM_VALUE local
#define HM_NAME local_map
#include "hash_map.h"

#define VEC_TYPE scope
#define VEC_TYPE_COPY scope_copy
#define VEC_TYPE_FREE scope_free
#include "vec.h"

static chunk *current_chunk(compiler *compiler) { return &compiler->chunk; }

/**
 * @param i The size in bytes to reserve
 */
static void reserve(compiler *compiler, size_t i) {
    uint8_t_vec_reserve(&compiler->chunk.instructions, i);
}

static void emit_byte(compiler *compiler, uint8_t byte) {
    uint8_t_vec_push(&current_chunk(compiler)->instructions, byte);
}

static void emit_byte2(compiler *compiler, uint8_t byte1, uint8_t byte2) {
    emit_byte(compiler, byte1);
    emit_byte(compiler, byte2);
}

static void emit_op(compiler *compiler, opcode op) {
    emit_byte(compiler, (uint8_t)op);
    compiler->stack_length += op_stack_effects[op];
}

static void emit_uint_16_t(compiler *compiler, uint16_t u) {
    reserve(compiler, sizeof(uint16_t));
    memcpy(&compiler->chunk.instructions.data[compiler->chunk.instructions.len],
           &u, sizeof(uint16_t));
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
    emit_byte2(compiler, OP_CONST, add_constant(compiler, value));
}

static void todo(compiler *compiler) {
    error(compiler, "Don't know how to compile this expression yet");
}

static void compile_bin_expr(compiler *compiler, expr_bin expr_bin) {
    compile_expr(compiler, expr_bin.left);
    compile_expr(compiler, expr_bin.right);
    switch (expr_bin.op) {
    case EXPR_OP_ADD: {
        emit_byte(compiler, OP_ADD);
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
            emit_byte(compiler, OP_TRUE);
        } else {
            emit_byte(compiler, OP_FALSE);
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
        emit_byte(compiler, OP_RET);
    } else {
        todo(compiler);
    }
}

compiler compiler_new(void) {
    return (compiler){
        .did_error = false,
        .stack_length = 0,
        .chunk = chunk_new(),
    };
}
