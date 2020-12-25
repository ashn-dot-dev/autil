#include <stdlib.h>
#include <string.h>
#define ATEST_IMPLEMENTATION
#include "../atest.h"

ATEST_TEST(test_foo)
{
    ATEST_ASSERT(42 != 314);
}

static ATEST_TEST(test_bar)
{
    ATEST_EXPECT(-1 == 3);
    ATEST_ASSERT(-1 == 5);
}

static ATEST_TEST(test_baz);
char const* somestr = NULL;
// clang-format off
static void baz_init(void) { somestr = "this is a string"; }
static void baz_fini(void) { somestr = NULL; }
// clang-format on

int
main(void)
{
    ATEST_RUN(test_foo);
    ATEST_RUN(test_bar);
    ATEST_RUNF(baz_init, baz_fini, test_baz);
    printf("TESTS RUN    : %d\n", atest_tests_run());
    printf("TESTS FAILED : %d\n", atest_tests_failed());
    return EXIT_SUCCESS;
}

static ATEST_TEST(test_baz)
{
    char const* const anotherstr = "this is another string";
    if (strcmp(somestr, "a different string") != 0) {
        ATEST_FAIL("Strings \"%s\" and \"%s\" differ!", somestr, anotherstr);
    }
}
