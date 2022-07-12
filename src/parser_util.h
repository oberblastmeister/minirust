#define ALLOC_EXPR(e) expr_arena_alloc(&ps->ast_arena.expr_arena, (e))

#define MAKE_EXPR_BIN(l, o, r)                                                 \
    (expr) {                                                                   \
        EXPR_BIN,                                                              \
            {.expr_bin = {                                                     \
                 .left = ALLOC_EXPR(l),                                        \
                 .op = (o),                                                    \
                 .right = ALLOC_EXPR(r),                                       \
             }},                                                               \
    }

#define MAKE_EXPR_UNARY(o, e)                                                  \
    (expr) {                                                                   \
        EXPR_UNARY,                                                            \
            {.expr_unary = {                                                   \
                 .expr = ALLOC_EXPR(e),                                        \
                 .op = (o),                                                    \
             }},                                                               \
    }

// #define PARSER_TYPE(t) typedef  
