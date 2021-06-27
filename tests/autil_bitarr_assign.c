#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bitarr* a = autil_bitarr_new(256);
    struct autil_bitarr* b = autil_bitarr_new(256);

    autil_bitarr_set(a, 0u, 1);
    autil_bitarr_set(a, 1u, 1);
    autil_bitarr_set(a, 5u, 1);
    autil_bitarr_set(a, 15u, 1);
    autil_bitarr_set(a, 64u, 1);
    autil_bitarr_set(a, 65u, 1);
    autil_bitarr_set(a, 100u, 1);
    autil_bitarr_set(a, 255u, 1);

    // Check nominal assignment.
    autil_bitarr_assign(b, a);
    for (size_t i = 0; i < autil_bitarr_count(a); ++i) {
        EXPECT(autil_bitarr_get(a, i) == autil_bitarr_get(b, i));
    }

    // Check self assignment.
    autil_bitarr_assign(a, a);
    autil_bitarr_assign(b, b);
    for (size_t i = 0; i < autil_bitarr_count(a); ++i) {
        EXPECT(autil_bitarr_get(a, i) == autil_bitarr_get(b, i));
    }

    autil_bitarr_del(a);
    autil_bitarr_del(b);

    // Make sure that arrays of different counts will cause an error.
    a = autil_bitarr_new(11);
    b = autil_bitarr_new(24);
    autil_bitarr_assign(b, a);

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
