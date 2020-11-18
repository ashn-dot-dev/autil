#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_vec* const v = autil_vec_new(sizeof(int));
    int const A = 0xBEEF;
    autil_vec_insert(v, 0, &A);
    ASSERT(AUTIL_DEREF_PTR(int, autil_vec_ref(v, 0)) == A);

    struct autil_vec const* ref = v;
    ASSERT(AUTIL_DEREF_PTR(int const, autil_vec_ref_const(ref, 0)) == A);

    // Make sure index out of bounds is correctly checked.
    autil_vec_ref(v, 1);

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
