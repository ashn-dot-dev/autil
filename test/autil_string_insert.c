#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string* const s = autil_string_new("");

    autil_string_insert(s, 0, "", 0);
    ASSERT(strcmp(autil_string_start(s), "") == 0);

    autil_string_insert(s, 0, "A", 1);
    ASSERT(strcmp(autil_string_start(s), "A") == 0);

    autil_string_insert(s, 1, "BCDX", 3);
    ASSERT(strcmp(autil_string_start(s), "ABCD") == 0);

    autil_string_insert(s, 1, "EF", 2);
    ASSERT(strcmp(autil_string_start(s), "AEFBCD") == 0);

    autil_string_insert(s, 0, NULL, 0);
    ASSERT(strcmp(autil_string_start(s), "AEFBCD") == 0);

    // Index out of bounds.
    autil_string_insert(s, 123, NULL, 0);

    autil_string_del(s);
}
