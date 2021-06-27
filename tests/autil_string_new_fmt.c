#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string* s = NULL;

    s = autil_string_new_fmt("foo");
    ASSERT(strcmp(autil_string_start(s), "foo") == 0);
    ASSERT(autil_string_count(s) == 3);
    autil_string_del(s);

    s = autil_string_new_fmt("%d %s", 123, "foo");
    ASSERT(strcmp(autil_string_start(s), "123 foo") == 0);
    ASSERT(autil_string_count(s) == 7);
    autil_string_del(s);

    s = autil_string_new_fmt("%.*s", 2, "bar");
    ASSERT(strcmp(autil_string_start(s), "ba") == 0);
    ASSERT(autil_string_count(s) == 2);
    autil_string_del(s);
}
