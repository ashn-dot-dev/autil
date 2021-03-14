#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bigint* const x = autil_bigint_new(AUTIL_BIGINT_ZERO);

    autil_bigint_magnitude_bit_set(x, 128, 0);
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 0);
    ASSERT(autil_bigint_bit_get(x, 4) == 0);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 0);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);

    autil_bigint_magnitude_bit_set(x, 0, 1);
    ASSERT(autil_bigint_bit_get(x, 0) == 1);
    ASSERT(autil_bigint_bit_get(x, 1) == 0);
    ASSERT(autil_bigint_bit_get(x, 4) == 0);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 0);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);

    autil_bigint_magnitude_bit_set(x, 0, 0);
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 0);
    ASSERT(autil_bigint_bit_get(x, 4) == 0);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 0);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);

    autil_bigint_magnitude_bit_set(x, 1, 1);
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 1);
    ASSERT(autil_bigint_bit_get(x, 4) == 0);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 0);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);

    autil_bigint_magnitude_bit_set(x, 4, 1);
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 1);
    ASSERT(autil_bigint_bit_get(x, 4) == 1);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 0);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);

    autil_bigint_magnitude_bit_set(x, 7, 1);
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 1);
    ASSERT(autil_bigint_bit_get(x, 4) == 1);
    ASSERT(autil_bigint_bit_get(x, 7) == 1);
    ASSERT(autil_bigint_bit_get(x, 8) == 0);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);

    autil_bigint_magnitude_bit_set(x, 8, 1);
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 1);
    ASSERT(autil_bigint_bit_get(x, 4) == 1);
    ASSERT(autil_bigint_bit_get(x, 7) == 1);
    ASSERT(autil_bigint_bit_get(x, 8) == 1);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);

    autil_bigint_magnitude_bit_set(x, 128, 1);
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 1);
    ASSERT(autil_bigint_bit_get(x, 4) == 1);
    ASSERT(autil_bigint_bit_get(x, 7) == 1);
    ASSERT(autil_bigint_bit_get(x, 8) == 1);
    ASSERT(autil_bigint_bit_get(x, 128) == 1);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);

    autil_bigint_magnitude_bit_set(x, 7, 0);
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 1);
    ASSERT(autil_bigint_bit_get(x, 4) == 1);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 1);
    ASSERT(autil_bigint_bit_get(x, 128) == 1);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);

    autil_bigint_magnitude_bit_set(x, 128, 0);
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 1);
    ASSERT(autil_bigint_bit_get(x, 4) == 1);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 1);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);

    struct autil_bigint* const y = autil_bigint_new_cstr("-0b10");
    autil_bigint_magnitude_bit_set(y, 0, 1);
    ASSERT(autil_bigint_bit_get(y, 0) == 1);
    ASSERT(autil_bigint_bit_get(y, 1) == 1);
    autil_bigint_magnitude_bit_set(y, 1, 0);
    ASSERT(autil_bigint_bit_get(y, 0) == 1);
    ASSERT(autil_bigint_bit_get(y, 1) == 0);
    autil_bigint_magnitude_bit_set(y, 0, 0);
    ASSERT(autil_bigint_bit_get(y, 0) == 0);
    ASSERT(autil_bigint_bit_get(y, 1) == 0);
    autil_bigint_magnitude_bit_set(y, 1, 1);
    ASSERT(autil_bigint_bit_get(y, 0) == 0);
    ASSERT(autil_bigint_bit_get(y, 1) == 1);

    autil_bigint_del(x);
    autil_bigint_del(y);
    return EXIT_SUCCESS;
}
