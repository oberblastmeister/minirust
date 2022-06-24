#include <criterion/criterion.h>

/* This is necessary on windows, as BoxFort needs the main to be exported
   in order to find it. */
#if defined(_WIN32) || defined(__CYGWIN__)
#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define DLLEXPORT __attribute__((dllexport))
#else
#error No dllexport attribute
#endif
#else
#define DLLEXPORT
#endif

DLLEXPORT int main(int argc, char *argv[]) {
    struct criterion_test_set *tests = criterion_initialize();

    int result = 0;
    if (criterion_handle_args(argc, argv, true))
        result = !criterion_run_all_tests(tests);

    criterion_finalize(tests);
    return result;
}

Test(misc, failing) { cr_assert(0); }

Test(misc, passing) { cr_assert(1); }
