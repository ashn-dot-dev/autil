#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

static void
test_valid(char const* input, char const* fmt)
{
    assert(input != NULL);

    struct autil_bigint* const num = autil_bigint_new_cstr(input);
    ASSERT(num != NULL);

    char* const cstr = autil_bigint_to_cstr(num, fmt);
    ASSERT(cstr != NULL);

    printf("%-26s | %-8s | \"%s\"\n", input, fmt ? fmt : "NULL", cstr);

    autil_bigint_del(num);
    autil_xalloc(cstr, AUTIL_XALLOC_FREE);
}

int
main(void)
{
    test_valid("+123456789123456789", NULL);
    test_valid("-123456789123456789", NULL);

    fputc('\n', stdout);
    test_valid("+123456789123456789", "d");
    test_valid("-123456789123456789", "d");

    fputc('\n', stdout);
    // Without padding these should do nothing.
    test_valid("+123456789123456789", "#d");
    test_valid("-123456789123456789", "#d");
    test_valid("+123456789123456789", "0d");
    test_valid("-123456789123456789", "0d");
    test_valid("+123456789123456789", " d");
    test_valid("-123456789123456789", " d");

    fputc('\n', stdout);
    test_valid("+123456789123456789", "+d");
    test_valid("-123456789123456789", "+d");
    test_valid("+123456789123456789", " d");
    test_valid("-123456789123456789", " d");

    fputc('\n', stdout);
    test_valid("+123456789123456789", "24d");
    test_valid("-123456789123456789", "24d");
    test_valid("+123456789123456789", "-24d");
    test_valid("-123456789123456789", "-24d");
    test_valid("+123456789123456789", "024d");
    test_valid("-123456789123456789", "024d");
    test_valid("+123456789123456789", "0-24d");
    test_valid("-123456789123456789", "0-24d");
    test_valid("+123456789123456789", " 24d");
    test_valid("-123456789123456789", " 24d");
    test_valid("+123456789123456789", " -24d");
    test_valid("-123456789123456789", " -24d");

    fputc('\n', stdout);
    test_valid("0b111100001100110010101010", "b");
    test_valid("0b111100001100110010101010", "#b");

    fputc('\n', stdout);
    test_valid("0xbeefbeefbeefbeef", "x");
    test_valid("0xbeefbeefbeefbeef", "X");
    test_valid("0xbeefbeefbeefbeef", "#x");
    test_valid("0xbeefbeefbeefbeef", "#X");

    struct autil_bigint* const num = autil_bigint_new_cstr("0xDEADBEEF");
    EXPECT(NULL == autil_bigint_to_cstr(num, ""));
    EXPECT(NULL == autil_bigint_to_cstr(num, "#"));
    EXPECT(NULL == autil_bigint_to_cstr(num, "0"));
    EXPECT(NULL == autil_bigint_to_cstr(num, "+"));
    EXPECT(NULL == autil_bigint_to_cstr(num, "-"));
    EXPECT(NULL == autil_bigint_to_cstr(num, " "));
    EXPECT(NULL == autil_bigint_to_cstr(num, "1"));
    autil_bigint_del(num);

    return EXIT_SUCCESS;
}
