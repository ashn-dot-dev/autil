#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string* const s = autil_string_new_cstr("");

    autil_string_append_cstr(s, "");
    ASSERT(strcmp(autil_string_start(s), "") == 0);

    autil_string_append_cstr(s, "FOO");
    ASSERT(strcmp(autil_string_start(s), "FOO") == 0);

    autil_string_append_cstr(s, "BAR");
    ASSERT(strcmp(autil_string_start(s), "FOOBAR") == 0);

    autil_string_append_cstr(s, "BA\0Z");
    ASSERT(strcmp(autil_string_start(s), "FOOBARBA") == 0);

    autil_string_append_cstr(s, "");
    ASSERT(strcmp(autil_string_start(s), "FOOBARBA") == 0);

    autil_string_del(s);
}
