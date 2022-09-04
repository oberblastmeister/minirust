#pragma once

// make sure the necessary imports are before #include "lexer.h"
// flex doesn't generate the header with the correct imports
#include "lexer_state.h"
#include "token.h"
#include "uint8_t_vec.h"
#include "vec_util.h"
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

void run_lexer_debug(void);
