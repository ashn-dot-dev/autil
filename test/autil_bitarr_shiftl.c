#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bitarr* res = autil_bitarr_new(256);
    struct autil_bitarr* lhs = autil_bitarr_new(256);

    autil_bitarr_set(lhs, 0u, 1);
    autil_bitarr_set(lhs, 1u, 1);
    autil_bitarr_set(lhs, 100u, 1);
    autil_bitarr_set(lhs, 254u, 1);
    autil_bitarr_set(lhs, 255u, 1);

    autil_bitarr_shiftl(res, lhs, 0u);
    ASSERT(autil_bitarr_get(res, 0u) == 1);
    ASSERT(autil_bitarr_get(res, 1u) == 1);
    ASSERT(autil_bitarr_get(res, 2u) == 0);
    ASSERT(autil_bitarr_get(res, 3u) == 0);
    ASSERT(autil_bitarr_get(res, 100u) == 1);
    ASSERT(autil_bitarr_get(res, 101u) == 0);
    ASSERT(autil_bitarr_get(res, 102u) == 0);
    ASSERT(autil_bitarr_get(res, 254u) == 1);
    ASSERT(autil_bitarr_get(res, 255u) == 1);

    autil_bitarr_shiftl(res, lhs, 1u);
    ASSERT(autil_bitarr_get(res, 0u) == 0);
    ASSERT(autil_bitarr_get(res, 1u) == 1);
    ASSERT(autil_bitarr_get(res, 2u) == 1);
    ASSERT(autil_bitarr_get(res, 3u) == 0);
    ASSERT(autil_bitarr_get(res, 100u) == 0);
    ASSERT(autil_bitarr_get(res, 101u) == 1);
    ASSERT(autil_bitarr_get(res, 102u) == 0);
    ASSERT(autil_bitarr_get(res, 254u) == 0);
    ASSERT(autil_bitarr_get(res, 255u) == 1);

    autil_bitarr_shiftl(res, lhs, 2u);
    ASSERT(autil_bitarr_get(res, 0u) == 0);
    ASSERT(autil_bitarr_get(res, 1u) == 0);
    ASSERT(autil_bitarr_get(res, 2u) == 1);
    ASSERT(autil_bitarr_get(res, 3u) == 1);
    ASSERT(autil_bitarr_get(res, 100u) == 0);
    ASSERT(autil_bitarr_get(res, 101u) == 0);
    ASSERT(autil_bitarr_get(res, 102u) == 1);
    ASSERT(autil_bitarr_get(res, 254u) == 0);
    ASSERT(autil_bitarr_get(res, 255u) == 0);

    autil_bitarr_shiftl(res, lhs, 256u);
    ASSERT(autil_bitarr_get(res, 0u) == 0);
    ASSERT(autil_bitarr_get(res, 1u) == 0);
    ASSERT(autil_bitarr_get(res, 2u) == 0);
    ASSERT(autil_bitarr_get(res, 3u) == 0);
    ASSERT(autil_bitarr_get(res, 100u) == 0);
    ASSERT(autil_bitarr_get(res, 101u) == 0);
    ASSERT(autil_bitarr_get(res, 102u) == 0);
    ASSERT(autil_bitarr_get(res, 254u) == 0);
    ASSERT(autil_bitarr_get(res, 255u) == 0);

    // Make sure that arrays of different counts will cause an error.
    res = autil_bitarr_new(64);
    lhs = autil_bitarr_new(32);
    autil_bitarr_shiftl(res, lhs, 0u);

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
