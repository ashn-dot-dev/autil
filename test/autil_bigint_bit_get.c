#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bigint* x = NULL;

    x = autil_bigint_new_cstr("0b0");
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 0);
    ASSERT(autil_bigint_bit_get(x, 4) == 0);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 0);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);
    autil_bigint_del(x);

    x = autil_bigint_new_cstr("0b1");
    ASSERT(autil_bigint_bit_get(x, 0) == 1);
    ASSERT(autil_bigint_bit_get(x, 1) == 0);
    ASSERT(autil_bigint_bit_get(x, 4) == 0);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 0);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);
    autil_bigint_del(x);

    x = autil_bigint_new_cstr("0b10");
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 1);
    ASSERT(autil_bigint_bit_get(x, 4) == 0);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 0);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);
    autil_bigint_del(x);

    x = autil_bigint_new_cstr("0b10000");
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 0);
    ASSERT(autil_bigint_bit_get(x, 4) == 1);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 0);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);
    autil_bigint_del(x);

    x = autil_bigint_new_cstr("0b100000000");
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 0);
    ASSERT(autil_bigint_bit_get(x, 4) == 0);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 1);
    ASSERT(autil_bigint_bit_get(x, 128) == 0);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);
    autil_bigint_del(x);

    x = autil_bigint_new_cstr("0x100000000000000000000000000000000");
    ASSERT(autil_bigint_bit_get(x, 0) == 0);
    ASSERT(autil_bigint_bit_get(x, 1) == 0);
    ASSERT(autil_bigint_bit_get(x, 4) == 0);
    ASSERT(autil_bigint_bit_get(x, 7) == 0);
    ASSERT(autil_bigint_bit_get(x, 8) == 0);
    ASSERT(autil_bigint_bit_get(x, 128) == 1);
    ASSERT(autil_bigint_bit_get(x, (size_t)-1) == 0);
    autil_bigint_del(x);

    struct autil_bigint* const y = autil_bigint_new_cstr("-0b10");
    ASSERT(autil_bigint_bit_get(y, 0) == 0);
    ASSERT(autil_bigint_bit_get(y, 1) == 1);
    autil_bigint_del(y);

    return EXIT_SUCCESS;
}
