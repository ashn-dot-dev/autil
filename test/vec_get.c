#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct vec* const v = vec_new(sizeof(int));
    int const A = 0xBEEF;
    vec_insert(v, 0, &A);
    ASSERT(DEREF_PTR(int, vec_get(v, 0)) == A);

    // Most of the other vec* function tests use vec_get, so this test is really
    // just going to make sure index out of bounds is correctly checked.
    vec_get(v, 1);

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
