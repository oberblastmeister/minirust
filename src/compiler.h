#pragma once

#include "ast.h"
#include "char_vec.h"
#include "chunk.h"

#define UINT8_COUNT (UINT8_MAX + 1)
#define UINT16_COUNT (UINT16_MAX + 1)

typedef struct {
    int stack_slot;
    // // may have all zero pattern
    // string name;
} local;

#define HM_KEY string
#define HM_KEY_COPY string_copy
#define HM_KEY_FREE string_free
#define HM_VALUE local
#define HM_NAME local_map
#include "hash_map_h.h"

#define VEC_TYPE local
#include "vec_h.h"

typedef struct {
    local_vec anon_locals;
    local_map named_locals;
} scope;

scope scope_copy(scope *scope);

void scope_free(scope *scope);

#define VEC_TYPE scope
#define VEC_TYPE_COPY scope_copy
#define VEC_TYPE_FREE scope_free
#include "vec_h.h"

typedef struct {
    bool did_error;
    int stack_length;
    // this also functions as giving each local a stack slot
    // this must start at 0, and the first local must have stack slot 0
    int local_count;
    scope_vec scopes;
    chunk chunk;
    obj *objects;
} compiler;

compiler compiler_new(void);

void compile_expr(compiler *compiler, expr *expr);

void compile_return(compiler *compiler, expr *expr);

void compiler_free(compiler *compiler);
