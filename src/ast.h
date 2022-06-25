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

void do_something(void) {
    autofree expr *left = malloc(sizeof(expr));
    *left = (expr){.tag = EXPR_CONST, .data = {.expr_const = 1}};
    autofree expr *right = malloc(sizeof(expr));
    *right = (expr){.tag = EXPR_CONST, .data = {.expr_const = 1}};
    expr expr = {.tag = EXPR_ADD,
                 .data = {
                     .expr_add = {.left = left, .right = right},
                 }};
}
