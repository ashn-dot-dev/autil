#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

static void
test(char const* start, size_t count, char const* expected)
{
    char* const s = autil_cstr_new(start, count);
    EXPECT(strcmp(s, expected) == 0);
    autil_xalloc(s, AUTIL_XALLOC_FREE);
}

static void
test_cstr(char const* cstr, char const* expected)
{
    char* const s = autil_cstr_new_cstr(cstr);
    EXPECT(strcmp(s, expected) == 0);
    autil_xalloc(s, AUTIL_XALLOC_FREE);
}

int
main(void)
{
    test("", 0, "");
    test("foo", 3, "foo");
    test("foo", 2, "fo");
    test("foo\0bar", 7, "foo");
    char* const s = autil_cstr_new("foo\0bar", 7);
    s[3] = '!';
    EXPECT(strcmp(s, "foo!bar") == 0);
    autil_xalloc(s, AUTIL_XALLOC_FREE);

    test_cstr("", "");
    test_cstr("foo", "foo");
    test_cstr("foo\nbar", "foo\nbar");
    test_cstr("foo\0bar", "foo");

    return EXIT_SUCCESS;
}
