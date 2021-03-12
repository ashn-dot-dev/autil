#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bitarr* const a = autil_bitarr_new(4096);

    ASSERT(autil_bitarr_get(a, 0u) == 0);
    ASSERT(autil_bitarr_get(a, 1u) == 0);
    ASSERT(autil_bitarr_get(a, 2u) == 0);
    ASSERT(autil_bitarr_get(a, 10u) == 0);
    ASSERT(autil_bitarr_get(a, 256u) == 0);
    ASSERT(autil_bitarr_get(a, 4095u) == 0);

    autil_bitarr_set(a, 0u, 1);
    ASSERT(autil_bitarr_get(a, 0u) == 1);
    ASSERT(autil_bitarr_get(a, 1u) == 0);

    autil_bitarr_set(a, 1u, 1);
    ASSERT(autil_bitarr_get(a, 0u) == 1);
    ASSERT(autil_bitarr_get(a, 1u) == 1);

    autil_bitarr_set(a, 4095u, 1);
    ASSERT(autil_bitarr_get(a, 0u) == 1);
    ASSERT(autil_bitarr_get(a, 1u) == 1);
    ASSERT(autil_bitarr_get(a, 2u) == 0);
    ASSERT(autil_bitarr_get(a, 10u) == 0);
    ASSERT(autil_bitarr_get(a, 256u) == 0);
    ASSERT(autil_bitarr_get(a, 4096) == 1);

    // Make sure index out of bounds is correctly checked.
    ASSERT(autil_bitarr_get(a, 4096u) == 0);

    puts("UNREACHABLE");
    autil_bitarr_del(a);
    return EXIT_SUCCESS;
}
