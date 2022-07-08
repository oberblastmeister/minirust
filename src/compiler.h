#ifndef _LOX_COMPILER_H
#define _LOX_COMPILER_H

#include "ast.h"
#include "char_vec.h"
#include "chunk.h"

#define UINT8_COUNT (UINT8_MAX + 1)

typedef struct {
    int id;
    // // may have all zero pattern
    // string name;
} local;

#define HM_KEY string
#define HM_KEY_COPY string_copy
#define HM_KEY_FREE string_free
#define HM_VALUE local
#define HM_NAME local_map
#include "hash_map_h.h"

typedef struct {
    int next_anon_id;
    local *anon_locals;
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
    int next_local_id;
    local locals[UINT8_COUNT];
    scope_vec scopes;
    chunk chunk;
} compiler;

compiler compiler_new(void);

void compile_expr(compiler *compiler, expr *expr);

void compile_return(compiler *compiler, expr *expr);

#endif
