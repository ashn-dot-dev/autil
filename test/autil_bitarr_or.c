#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bitarr* res = autil_bitarr_new(256);
    struct autil_bitarr* lhs = autil_bitarr_new(256);
    struct autil_bitarr* rhs = autil_bitarr_new(256);

    for (size_t i = 64; i < 128; ++i) {
        autil_bitarr_set(lhs, i, 1);
    }
    for (size_t i = 64; i < 192; ++i) {
        autil_bitarr_set(rhs, i, 1);
    }
    for (size_t i = 192; i < 256; ++i) {
        autil_bitarr_set(lhs, i, 1);
        autil_bitarr_set(rhs, i, 1);
    }

    autil_bitarr_or(res, lhs, rhs);
    for (size_t i = 0; i < autil_bitarr_count(res); ++i) {
        EXPECT(
            autil_bitarr_get(res, i)
            == (autil_bitarr_get(lhs, i) || autil_bitarr_get(rhs, i)));
    }

    autil_bitarr_del(res);
    autil_bitarr_del(lhs);
    autil_bitarr_del(rhs);

    // Make sure that arrays of different counts will cause an error.
    res = autil_bitarr_new(64);
    lhs = autil_bitarr_new(32);
    rhs = autil_bitarr_new(16);
    autil_bitarr_or(res, lhs, rhs);

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
