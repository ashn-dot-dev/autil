#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string* const s = autil_string_new_cstr("");

    autil_string_append(s, "", 0);
    ASSERT(strcmp(autil_string_start(s), "") == 0);

    autil_string_append(s, "FOO", 3);
    ASSERT(strcmp(autil_string_start(s), "FOO") == 0);

    autil_string_append(s, "BAR", 3);
    ASSERT(strcmp(autil_string_start(s), "FOOBAR") == 0);

    autil_string_append(s, "BAZ", 2);
    ASSERT(strcmp(autil_string_start(s), "FOOBARBA") == 0);

    autil_string_append(s, "", 0);
    ASSERT(strcmp(autil_string_start(s), "FOOBARBA") == 0);

    autil_string_del(s);
}
