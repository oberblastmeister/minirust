#ifndef _LOX_VM_H
#define _LOX_VM_H

#include "chunk.h"

#define STACK_MAX 256

typedef struct {
    chunk chunk;
    uint8_t *ip;
    value stack[STACK_MAX];
    value *stack_top;
} vm;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} interpret_result;

vm vm_new(chunk chunk);

void vm_free(vm *vm);

interpret_result vm_run(vm *vm);

void vm_push(vm *vm, value value);

value vm_pop(vm *vm);

void vm_print_stack(vm *vm);

#endif
