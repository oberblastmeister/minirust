#ifndef _LOX_TEST_BITS_C
#define _LOX_TEST_BITS_C

#include "../src/bits.h"
#include <criterion/criterion.h>

Test(bits, next_power_of_2) { cr_assert_eq(next_power_of_2(5), 8); }

#else
#error "Included twice"
#endif
