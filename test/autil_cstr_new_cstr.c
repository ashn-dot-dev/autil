#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

static void
test(char const* cstr, char const* expected)
{
    char* const s = autil_cstr_new_cstr(cstr);
    EXPECT(strcmp(s, expected) == 0);
    autil_xalloc(s, AUTIL_XALLOC_FREE);
}

int
main(void)
{
    test("", "");
    test("foo", "foo");
    test("foo\nbar", "foo\nbar");
    test("foo\0bar", "foo");

    return EXIT_SUCCESS;
}
