#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string* const s = autil_string_new("ABCDEF");

    autil_string_remove(s, 0, 0);
    ASSERT(strcmp(autil_string_start(s), "ABCDEF") == 0);

    autil_string_remove(s, 0, 1);
    ASSERT(strcmp(autil_string_start(s), "BCDEF") == 0);

    autil_string_remove(s, 2, 2);
    ASSERT(strcmp(autil_string_start(s), "BCF") == 0);

    // Remove bytes that slice out of bounds.
    autil_string_remove(s, 0, 4);
    ASSERT(strcmp(autil_string_start(s), "BCF") == 0);

    autil_string_del(s);
}
