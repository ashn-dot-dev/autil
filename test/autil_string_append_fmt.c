#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string* const s = autil_string_new_cstr("");

    autil_string_append_fmt(s, "");
    ASSERT(strcmp(autil_string_start(s), "") == 0);

    autil_string_append_fmt(s, "%d %s", 123, "foo");
    ASSERT(strcmp(autil_string_start(s), "123 foo") == 0);

    autil_string_append_fmt(s, "%.*s", 2, "bar");
    ASSERT(strcmp(autil_string_start(s), "123 fooba") == 0);

    autil_string_append_fmt(s, "");
    ASSERT(strcmp(autil_string_start(s), "123 fooba") == 0);

    autil_string_del(s);
}
