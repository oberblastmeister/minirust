#include "vm.h"
#include "io_extra.h"
#include "value.h"
#include <stdio.h>

vm vm_new(chunk chunk) {
    vm vm = {.chunk = chunk, .ip = chunk.instructions.data};
    vm.stack_top = vm.stack;
    return vm;
}

void vm_free(vm *vm) { chunk_free(&vm->chunk); }

interpret_result vm_run(vm *vm) {
#define BINARY_OP(vm, op)                                                      \
    do {                                                                       \
        value b = vm_pop(vm);                                                  \
        value a = vm_pop(vm);                                                  \
        vm_push(vm, a op b);                                                   \
    } while (false)

    while (true) {
        uint8_t instruction;
        switch (instruction = *vm->ip++) {
        case OP_CONSTANT: {
            value constant = value_vec_index(&vm->chunk.constants, *vm->ip++);
            vm_push(vm, constant);
            break;
        }
        case OP_ADD: {
            // BINARY_OP(vm, +);
            break;
        }
        case OP_SUBTRACT: {
            // BINARY_OP(vm, -);
            break;
        }
        case OP_MULTIPLY: {
            // BINARY_OP(vm, *);
            break;
        }
        case OP_DIVIDE: {
            // BINARY_OP(vm, /);
            break;
        }
        case OP_NEGATE: {
            // vm_push(vm, -vm_pop(vm));
            break;
        }
        case OP_RETURN: {
            value_print(vm_pop(vm));
            puts("");
            return INTERPRET_OK;
        }
        }
    }
#undef BINARY_OP
}

void vm_push(vm *vm, value value) {
    *vm->stack_top = value;
    vm->stack_top++;
}

value vm_pop(vm *vm) {
    vm->stack_top--;
    return *vm->stack_top;
}

void vm_print_stack(vm *vm) {
    put("          ");
    for (value *slot = vm->stack; slot < vm->stack_top; slot++) {
        put("[ ");
        value_print(*slot);
        put(" ]");
    }
    puts("");
}
