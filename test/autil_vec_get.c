#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_vec* const v = autil_vec_new(sizeof(int));
    int const A = 0xBEEF;
    autil_vec_insert(v, 0, &A);
    ASSERT(AUTIL_DEREF_PTR(int, autil_vec_get(v, 0)) == A);

    // Most of the other autil_vec* function tests use autil_vec_get, so this
    // test is really just going to make sure index out of bounds is correctly
    // checked.
    autil_vec_get(v, 1);

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
