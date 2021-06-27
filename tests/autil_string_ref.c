#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string* const s = autil_string_new_cstr("foobar");

    ASSERT(*autil_string_ref(s, 0) == 'f');
    ASSERT(*autil_string_ref(s, 1) == 'o');
    ASSERT(*autil_string_ref(s, 2) == 'o');
    ASSERT(*autil_string_ref(s, 3) == 'b');
    ASSERT(*autil_string_ref(s, 4) == 'a');
    ASSERT(*autil_string_ref(s, 5) == 'r');

    *autil_string_ref(s, 2) = 'A';
    ASSERT(*autil_string_ref(s, 2) == 'A');

    // Check index out of bounds.
    autil_string_ref(s, 6);
}
