#include "opcode.h"

const int op_stack_effects[] = {
#define X(_, effect, __) effect,
    OPCODE_LIST_X
#undef X
};

const int op_arg_bytes[] = {
#define X(_, __, args) args,
    OPCODE_LIST_X
#undef X
};

const char *op_names[] = {
#define X(name, _, __) #name,
    OPCODE_LIST_X
#undef X
};

// clang-format off
const int op_amount =
#define X(_, __, ___) 1 +
    OPCODE_LIST_X 0
#undef X
;
// clang-format on
