#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string const* const s = autil_string_new_cstr("foobar");

    ASSERT(*autil_string_ref_const(s, 0) == 'f');
    ASSERT(*autil_string_ref_const(s, 1) == 'o');
    ASSERT(*autil_string_ref_const(s, 2) == 'o');
    ASSERT(*autil_string_ref_const(s, 3) == 'b');
    ASSERT(*autil_string_ref_const(s, 4) == 'a');
    ASSERT(*autil_string_ref_const(s, 5) == 'r');

    // Check index out of bounds.
    autil_string_ref_const(s, 6);
}
