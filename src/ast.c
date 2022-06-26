#include "ast.h"
#include "memory.h"

void test(void) {}

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
