#ifndef _LOX_OPCODE_H
#define _LOX_OPCODE_H

#define OPCODE_LIST_X                                                          \
    X(CONST, 1)                                                                \
    X(CONST_16, 1)                                                             \
    X(ADD, -1)                                                                 \
    X(SUB, -1)                                                                 \
    X(MUL, -1)                                                                 \
    X(DIV, -1)                                                                 \
    X(NEG, 0)                                                                  \
    X(RET, 0)                                                                  \
    X(TRUE, 1)                                                                 \
    X(FALSE, 1)

typedef enum {
#define X(name, _) OP_##name,
    OPCODE_LIST_X
#undef X
} opcode;

extern const int op_stack_effects[];

#endif
