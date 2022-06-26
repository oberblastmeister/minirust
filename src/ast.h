#include "memory.h"
#include <stdlib.h>

typedef struct expr expr;
struct expr {
    enum { EXPR_CONST, EXPR_ADD, EXPR_SUB } tag;
    union {
        int expr_const;
        struct {
            expr *left;
            expr *right;
        } expr_add;
        struct {
            expr *expr;
        } expr_sub;
    } data;
};
