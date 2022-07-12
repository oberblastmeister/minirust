#ifndef _LOX_OPCODE_H
#define _LOX_OPCODE_H

#define OPCODE_LIST_X                                                          \
    X(CONST, 1, 1)                                                             \
    X(CONST_16, 1, 2)                                                          \
    X(NIL, 1, 0)                                                               \
    X(ADD, -1, 0)                                                              \
    X(SUB, -1, 0)                                                              \
    X(MUL, -1, 0)                                                              \
    X(DIV, -1, 0)                                                              \
    X(NEG, 0, 0)                                                               \
    X(EQ, -1, 0)                                                               \
    X(NEQ, -1, 0)                                                              \
    X(RET, 0, 0)                                                               \
    X(FALSE, 1, 0)                                                             \
    X(TRUE, 1, 0)                                                              \
    X(POP, -1, 0)                                                              \
    /* special opcode the stack is                                             \
    changed by n, 0 is just a placeholder */                                   \
    X(POP_N, 0, 1)                                                             \
    X(POP_N_16, 0, 2)                                                          \
    X(LOAD_LOCAL, 1, 1)                                                        \
    X(LOAD_LOCAL_16, 1, 2)                                                     \
    X(STORE_LOCAL, -1, 1)                                                      \
    X(STORE_LOCAL_16, -1, 2)

typedef enum {
#define X(name, _, __) OP_##name,
    OPCODE_LIST_X
#undef X
} opcode;

extern const int op_stack_effects[];

extern const int op_arg_bytes[];

extern const char *op_names[];

extern const int op_amount;

#endif
