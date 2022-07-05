#include "../src/ghash.h"
#include "../src/geq.h"
#include <criterion/criterion.h>

Test(ghash, ghash_simple) {
    int i = 1;
    int j = 1;
    int k = 2;
    cr_assert_eq(geq(&i, &j), 1);
    cr_assert_eq(geq(&i, &k), 0);
}
