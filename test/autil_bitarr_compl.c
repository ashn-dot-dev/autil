#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bitarr* res = autil_bitarr_new(256);
    struct autil_bitarr* rhs = autil_bitarr_new(256);

    autil_bitarr_compl(res, res);
    for (size_t i = 0; i < autil_bitarr_count(res); ++i) {
        ASSERT(autil_bitarr_get(res, i) == 1);
    }

    autil_bitarr_compl(res, res);
    for (size_t i = 0; i < autil_bitarr_count(res); ++i) {
        ASSERT(autil_bitarr_get(res, i) == 0);
    }

    autil_bitarr_set(rhs, 0u, 1);
    autil_bitarr_set(rhs, 4u, 1);
    autil_bitarr_set(rhs, 7u, 1);
    autil_bitarr_set(rhs, 32u, 1);
    autil_bitarr_set(rhs, 255u, 1);
    autil_bitarr_compl(res, rhs);
    for (size_t i = 0; i < autil_bitarr_count(res); ++i) {
        ASSERT(autil_bitarr_get(res, i) == !autil_bitarr_get(rhs, i));
    }

    autil_bitarr_del(res);
    autil_bitarr_del(rhs);

    // Make sure that arrays of different counts will cause an error.
    res = autil_bitarr_new(64);
    rhs = autil_bitarr_new(32);
    autil_bitarr_compl(res, rhs);

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
