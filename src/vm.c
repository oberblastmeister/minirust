#include "vm.h"
#include "io_ext.h"
#include "opcode.h"
#include "read_bytes.h"
#include "value.h"
#include <stdarg.h>
#include <stdio.h>

vm vm_new(chunk chunk) {
    vm vm = {.chunk = chunk,
             .ip = chunk.instructions.data,
             .stack = calloc(STACK_MAX, sizeof(value))};
    vm.stack_top = vm.stack;
    return vm;
}

void vm_free(vm *vm) { chunk_free(&vm->chunk); }

static value vm_peek(vm *vm, int distance) {
    return vm->stack_top[-1 - distance];
}

static void runtime_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);
}

value vm_pop(vm *vm) {
    vm->stack_top--;
    return *vm->stack_top;
}

static inline void vm_pop_n(vm *vm, int n) { vm->stack_top -= n; }

#define RUNTIME_ERROR(...)                                                     \
    {                                                                          \
        runtime_error(__VA_ARGS__);                                            \
        return INTERPRET_RUNTIME_ERROR;                                        \
    }                                                                          \
    struct _useless_struct_to_force_semicolon

interpret_result vm_run(vm *vm) {
#define BINARY_OP(vm, op)                                                      \
    do {                                                                       \
        value b = vm_pop(vm);                                                  \
        value a = vm_pop(vm);                                                  \
        vm_push(vm, a op b);                                                   \
    } while (false)
#define READ_BYTE() *vm->ip++
#define READ_T(T)                                                              \
    ({                                                                         \
        T t = read_##T((char *)vm->ip);                                        \
        vm->ip += sizeof(T);                                                   \
        t;                                                                     \
    })

    while (true) {
        uint8_t instruction = READ_BYTE();
        switch (instruction) {
        case OP_NIL: {
            vm_push(vm, value_nil());
            break;
        }
        case OP_TRUE: {
            vm_push(vm, value_bool(true));
            break;
        }
        case OP_FALSE: {
            vm_push(vm, value_bool(false));
            break;
        }
        case OP_CONST: {
            value constant = value_vec_index(&vm->chunk.constants, READ_BYTE());
            vm_push(vm, constant);
            break;
        }
        case OP_ADD: {
            if (value_is_double(vm_peek(vm, 0)) &&
                value_is_double(vm_peek(vm, 1))) {
                double b = value_as_double(vm_pop(vm));
                double a = value_as_double(vm_pop(vm));
                vm_push(vm, value_double(a + b));
            } else if (value_is_int(vm_peek(vm, 0)) &&
                       value_is_int(vm_peek(vm, 1))) {
                int b = value_as_int(vm_pop(vm));
                int a = value_as_int(vm_pop(vm));
                vm_push(vm, value_int(a + b));
            } else {
                RUNTIME_ERROR("incorrect operands for add");
            }
            break;
        }
        case OP_SUB: {
            // BINARY_OP(vm, -);
            break;
        }
        case OP_MUL: {
            // BINARY_OP(vm, *);
            break;
        }
        case OP_DIV: {
            // BINARY_OP(vm, /);
            break;
        }
        case OP_NEG: {
            // vm_push(vm, -vm_pop(vm));
            break;
        }
        case OP_LOAD_LOCAL: {
            uint8_t slot = READ_BYTE();
            vm_push(vm, vm->stack[slot]);
            break;
        }
        case OP_LOAD_LOCAL_16: {
            vm_push(vm, vm->stack[READ_T(uint16_t)]);
            break;
        }
        case OP_STORE_LOCAL: {
            value v = vm_pop(vm);
            uint8_t slot = READ_BYTE();
            vm->stack[slot] = v;
            break;
        }
        case OP_STORE_LOCAL_16: {
            vm->stack[READ_T(uint16_t)] = vm_pop(vm);
            break;
        }
        case OP_POP: {
            vm_pop(vm);
            break;
        }
        case OP_POP_N: {
            vm_pop_n(vm, (int)READ_BYTE());
            break;
        }
        case OP_POP_N_16: {
            vm_pop_n(vm, (int)READ_T(uint16_t));
            break;
        }
        case OP_RET: {
            puts("Got return");
            value_print(vm_pop(vm));
            puts("");
            return INTERPRET_OK;
        }
        default: {
            if (instruction > op_amount) {
                RUNTIME_ERROR("Invalid opcode with number %d", instruction);
            }
            RUNTIME_ERROR("Don't know how to interpret opcode %s",
                          op_names[instruction]);
        }
        }
    }
#undef BINARY_OP
#undef READ_BYTE
#undef READ_T
}

void vm_push(vm *vm, value value) {
    *vm->stack_top = value;
    vm->stack_top++;
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
