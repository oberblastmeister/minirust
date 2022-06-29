#include "opcode.h"

const int op_stack_effects[] = {
#define X(_, effect) effect,
    OPCODE_LIST_X
#undef x
};
