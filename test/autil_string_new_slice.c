#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string* s = NULL;

    s = autil_string_new_slice(NULL, 0);
    ASSERT(strcmp(autil_string_start(s), "") == 0);
    ASSERT(autil_string_count(s) == 0);
    autil_string_del(s);

    s = autil_string_new_slice("", 0);
    ASSERT(strcmp(autil_string_start(s), "") == 0);
    ASSERT(autil_string_count(s) == 0);
    autil_string_del(s);

    s = autil_string_new_slice("foobar", AUTIL_CSTR_COUNT("foobar"));
    ASSERT(strcmp(autil_string_start(s), "foobar") == 0);
    ASSERT(autil_string_count(s) == strlen("foobar"));
    autil_string_del(s);

    s = autil_string_new_slice("foo\0bar", AUTIL_CSTR_COUNT("foo\0bar"));
    ASSERT(strcmp(autil_string_start(s), "foo") == 0);
    ASSERT(
        memcmp(autil_string_start(s), "foo\0bar", AUTIL_CSTR_COUNT("foo\0bar"))
        == 0);
    autil_string_del(s);
}
