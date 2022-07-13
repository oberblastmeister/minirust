#include "compiler.h"
#include "ast.h"
#include "object.h"
#include "opcode.h"
#include "stdarg.h"
#include <stdio.h>
#include <string.h>

#define VEC_TYPE local
#include "vec.h"

static scope scope_new(void) {
    return (scope){
        .anon_locals = local_vec_new(),
        .named_locals = local_map_new(),
    };
}

scope scope_copy(scope *old_scope) {
    return (scope){
        .anon_locals = local_vec_copy(&old_scope->anon_locals),
        .named_locals = local_map_copy(&old_scope->named_locals),
    };
}

void scope_free(scope *scope) {
    local_vec_free(&scope->anon_locals);
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

static inline void emit_op(compiler *compiler, opcode op) {
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

static int add_constant(compiler *compiler, value value) {
    int constant = chunk_add_constant(current_chunk(compiler), value);
    if (constant > UINT8_MAX) {
        error(compiler, "Too many constants in one chunk.");
        return 0;
    }
    return constant;
}

static void todo(compiler *compiler) {
    error(compiler, "Don't know how to compile this expression yet");
}

static local create_local(compiler *compiler) {
    return (local){.stack_slot = compiler->local_count++};
}

static void compile_stmt_let(compiler *compiler, stmt_let stmt_let) {
    compile_expr(compiler, stmt_let.expr);
    scope *scope = scope_vec_last(&compiler->scopes);
    string name = stmt_let.name;
    if (local_map_contains(&scope->named_locals, &name)) {
        // TODO: allow shadowing
        error(compiler, "Locals cannot be shadowed in the same scope");
        return;
    }
    local_map_insert(&scope->named_locals, name, create_local(compiler));
}

static local compile_anon_local(compiler *compiler) {
    emit_op(compiler, OP_NIL);
    scope *scope = scope_vec_last(&compiler->scopes);
    local local = create_local(compiler);
    local_vec_push(&scope->anon_locals, local);
    printf("anon with: %d\n", local.stack_slot);
    return local;
}

#define EMIT_VAR_SIZED_OPCODE(compiler, op, arg, error_msg)                    \
    {                                                                          \
        if (arg > UINT16_COUNT) {                                              \
            error(compiler, error_msg);                                        \
            return;                                                            \
        }                                                                      \
        if (arg > UINT8_COUNT) {                                               \
            emit_op(compiler, op##_16);                                        \
            emit_uint_16_t(compiler, (uint16_t)arg);                           \
            return;                                                            \
        }                                                                      \
        emit_op(compiler, op);                                                 \
        emit_byte(compiler, (uint8_t)arg);                                     \
    }

static void emit_load_local(compiler *compiler, local local) {
    EMIT_VAR_SIZED_OPCODE(compiler, OP_LOAD_LOCAL, local.stack_slot,
                          "Too many locals");
}

static void emit_store_local(compiler *compiler, local local) {
    EMIT_VAR_SIZED_OPCODE(compiler, OP_STORE_LOCAL, local.stack_slot,
                          "Too many locals");
}

static void emit_pop_n(compiler *compiler, int n) {
    EMIT_VAR_SIZED_OPCODE(compiler, OP_POP_N, n, "Too many locals to pop");
    compiler->stack_length -= n;
}

static void emit_constant(compiler *compiler, value value) {
    int i = add_constant(compiler, value);
    EMIT_VAR_SIZED_OPCODE(compiler, OP_CONST, i, "Too many constants");
}

static void begin_scope(compiler *compiler) {
    printf("begin_scope\n");
    scope scope = scope_new();
    scope_vec_push(&compiler->scopes, scope);
}

static void end_scope(compiler *compiler) {
    scope scope = scope_vec_pop(&compiler->scopes);
    // TODO: we need to account for same scope shadowing
    size_t num_locals = scope.anon_locals.len + scope.named_locals.len;
    printf("emitting popn: %ld\n", num_locals);
    emit_pop_n(compiler, num_locals);
    scope_free(&scope);
    compiler->local_count -= (int)num_locals;
}

/**
 * @return The pointer to the local. May be NULL.
 */
static local *resolve_local(compiler *compiler, string *name) {
    for (size_t i = compiler->scopes.len; i-- > 0;) {
        scope *scope = &compiler->scopes.data[i];
        local *local = local_map_get_ptr(&scope->named_locals, name);
        if (local != NULL) {
            return local;
        }
    }
    return NULL;
}

static void compile_stmt_expr(compiler *compiler, expr *expr) {
    compile_expr(compiler, expr);
    emit_op(compiler, OP_POP);
}

static void compile_stmt_set(compiler *compiler, stmt_set set) {
    compile_expr(compiler, set.expr);
    lvalue lvalue = set.lvalue;
    switch (lvalue.tag) {
    case LVALUE_IDENT: {
        local *local = resolve_local(compiler, &lvalue.lvalue_ident);
        if (local == NULL) {
            error(compiler, "Could not resolve variable in set statement");
            break;
        }
        emit_store_local(compiler, *local);
        break;
    }
    }
}

static void compile_stmt(compiler *compiler, stmt stmt) {
    switch (stmt.tag) {
    case STMT_EXPR: {
        compile_stmt_expr(compiler, stmt.data.stmt_expr);
        break;
    }
    case STMT_LET: {
        compile_stmt_let(compiler, stmt.data.stmt_let);
        break;
    }
    case STMT_SET: {
        compile_stmt_set(compiler, stmt.data.stmt_set);
        break;
    }
    }
}

static void compile_expr_block(compiler *compiler, expr_block expr_block) {
    if (expr_block.last != NULL) {
        local local = compile_anon_local(compiler);
        begin_scope(compiler);
        for (size_t i = 0; i < expr_block.stmts.len; i++) {
            compile_stmt(compiler, expr_block.stmts.data[i]);
        }
        compile_expr(compiler, expr_block.last);
        emit_store_local(compiler, local);
        end_scope(compiler);
        emit_load_local(compiler, local);
    } else {
        begin_scope(compiler);
        for (size_t i = 0; i < expr_block.stmts.len; i++) {
            compile_stmt(compiler, expr_block.stmts.data[i]);
        }
        end_scope(compiler);
        emit_op(compiler, OP_NIL);
    }
}

static void compile_bin_expr(compiler *compiler, expr_bin expr_bin) {
    compile_expr(compiler, expr_bin.left);
    compile_expr(compiler, expr_bin.right);
    switch (expr_bin.op) {
#define SIMPLE(OP_AST, OPCODE)                                                 \
    case OP_AST: {                                                             \
        emit_op(compiler, OPCODE);                                             \
        break;                                                                 \
    }

        // clang-format off
    SIMPLE(EXPR_OP_ADD, OP_ADD)
    SIMPLE(EXPR_OP_SUB, OP_SUB)
    SIMPLE(EXPR_OP_MUL, OP_MUL)
    SIMPLE(EXPR_OP_DIV, OP_DIV)
    SIMPLE(EXPR_OP_EQ, OP_EQ)
    SIMPLE(EXPR_OP_NEQ, OP_NEQ)
        // clang-format on

#undef SIMPLE
    case EXPR_OP_LT: {
    }
    default: {
        todo(compiler);
        break;
    }
    }
}

void compile_expr(compiler *compiler, expr *expr) {
    switch (expr->tag) {
    case EXPR_NIL: {
        emit_op(compiler, OP_NIL);
        break;
    }
    case EXPR_INT: {
        emit_constant(compiler, value_int(expr->data.expr_int));
        break;
    }
    case EXPR_DOUBLE: {
        emit_constant(compiler, value_double(expr->data.expr_double));
        break;
    }
    case EXPR_STRING: {
        string s = expr->data.expr_string;
        emit_constant(compiler, value_obj((obj *)obj_string_new(
                                    s.data, s.len, &compiler->objects)));
        break;
    }
    case EXPR_BOOL: {
        emit_op(compiler, OP_FALSE + expr->data.expr_bool);
        break;
    }
    case EXPR_BIN: {
        compile_bin_expr(compiler, expr->data.expr_bin);
        break;
    }
    case EXPR_IDENT: {
        local *local = resolve_local(compiler, &expr->data.expr_ident);
        if (local == NULL) {
            error(compiler, "Could not resolve variable");
            break;
        }
        emit_load_local(compiler, *local);
        break;
    }
    case EXPR_BLOCK: {
        compile_expr_block(compiler, expr->data.expr_block);
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
        emit_op(compiler, OP_RET);
    } else {
        todo(compiler);
    }
}

compiler compiler_new(void) {
    scope_vec scopes = scope_vec_new();
    // initial scope
    scope_vec_push(&scopes, scope_new());
    return (compiler){false, 0, 0, scopes, chunk_new(), NULL};
}

void compiler_free(compiler *compiler) { scope_vec_free(&compiler->scopes); }
