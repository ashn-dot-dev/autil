#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bigint* x = NULL;

    x = autil_bigint_new_cstr("0b0");
    ASSERT(autil_bigint_bit_count(x) == 0);
    autil_bigint_del(x);

    x = autil_bigint_new_cstr("0b1");
    ASSERT(autil_bigint_bit_count(x) == 1);
    for (size_t i = 2; i <= 1000; ++i) {
        autil_bigint_magnitude_shiftl(x, 1);
        ASSERT(autil_bigint_bit_count(x) == i);
    }
    autil_bigint_del(x);

    x = autil_bigint_new_cstr("0b101");
    ASSERT(autil_bigint_bit_count(x) == 3);
    autil_bigint_shiftr(x, 1);
    ASSERT(autil_bigint_bit_count(x) == 2);
    autil_bigint_shiftr(x, 1);
    ASSERT(autil_bigint_bit_count(x) == 1);
    autil_bigint_shiftr(x, 1);
    ASSERT(autil_bigint_bit_count(x) == 0);
    autil_bigint_del(x);

    return EXIT_SUCCESS;
}
