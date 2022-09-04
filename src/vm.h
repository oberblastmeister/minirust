#pragma once

#include "chunk.h"
#include "object.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    obj_fun *fun;
    uint8_t *ip;
    value *stack;
} call_frame;

#define VEC_TYPE call_frame
#include "vec_h.h"

typedef struct {
    call_frame_vec frames;
    chunk chunk;
    uint8_t *ip;
    value *stack;
    value *stack_top;
    obj *objects;
} vm;

typedef enum {
    INTERPRET_OK,
    INTERPRET_PARSE_ERROR,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} interpret_result;

vm vm_new(chunk chunk, obj *objects);

void vm_free(vm *vm);

interpret_result vm_run(vm *vm);

void vm_push(vm *vm, value value);

value vm_pop(vm *vm);

void vm_print_stack(vm *vm);
