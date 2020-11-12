#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

static void
test_valid(char const* input, char const* fmt)
{
    assert(input != NULL);
    assert(fmt != NULL);

    struct autil_bigint* const num = autil_bigint_new_cstr(input);
    ASSERT(num != NULL);

    char* const cstr = autil_bigint_to_cstr(num, fmt);
    ASSERT(cstr != NULL);

    ASSERT((strlen(cstr) <= 36) && "NEED TO INCREASE PRINTF FORMAT LENGTH");
    printf("%-36s | %s\n", input, cstr);

    autil_bigint_del(num);
    autil_xalloc(cstr, AUTIL_XALLOC_FREE);
}

int
main(void)
{
    printf("%-36s | %s\n", "INPUT", "OUTPUT");

    test_valid("0", "#+d");
    test_valid("+0", "#+d");
    test_valid("-0", "#+d");

    test_valid("1", "#+d");
    test_valid("+1", "#+d");
    test_valid("-1", "#+d");
    test_valid("0b1", "#+b");
    test_valid("+0b1", "#+b");
    test_valid("-0b1", "#+b");
    test_valid("0x1", "#+x");
    test_valid("+0x1", "#+x");
    test_valid("-0x1", "#+x");

    test_valid("0b111100001100110010101010", "#+b");
    test_valid("+0b111100001100110010101010", "#+b");
    test_valid("-0b111100001100110010101010", "#+b");

    test_valid("0xDEADBEEF", "#+x");
    test_valid("+0xDEADBEEF", "#+x");
    test_valid("-0xDEADBEEF", "#+x");
    test_valid("0xcafed00d", "#+x");
    test_valid("0xcafebabe", "#+x");
    test_valid("0xbeef0000", "#+x");
    test_valid("0xBEEFBEEFBEEFBEEFBEEFBEEFBEEFBEEF", "#+x");

    test_valid("00", "#+d");
    test_valid("00000000000000000000000000000000", "#+d");

    EXPECT(NULL == autil_bigint_new_cstr(""));
    EXPECT(NULL == autil_bigint_new_cstr("+"));
    EXPECT(NULL == autil_bigint_new_cstr("-"));
    EXPECT(NULL == autil_bigint_new_cstr(" "));
    EXPECT(NULL == autil_bigint_new_cstr(" 1"));
    EXPECT(NULL == autil_bigint_new_cstr("1 "));
    EXPECT(NULL == autil_bigint_new_cstr(" 1 "));
    EXPECT(NULL == autil_bigint_new_cstr("1.1"));

    return EXIT_SUCCESS;
}
