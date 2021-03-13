#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    struct autil_bigint* x = autil_bigint_new_cstr("0xff0000");
    autil_bigint_dump(x);

    autil_bigint_magnitude_shiftr(x, 0);
    autil_bigint_dump(x);

    autil_bigint_magnitude_shiftr(x, 4);
    autil_bigint_dump(x);

    autil_bigint_magnitude_shiftr(x, 7);
    autil_bigint_dump(x);

    autil_bigint_magnitude_shiftr(x, 1);
    autil_bigint_dump(x);

    autil_bigint_magnitude_shiftr(x, 32);
    autil_bigint_dump(x);

    autil_bigint_magnitude_shiftr(x, 4096);
    autil_bigint_dump(x);

    struct autil_bigint* const y = autil_bigint_new_cstr("-0b10");
    autil_bigint_dump(y);
    autil_bigint_magnitude_shiftr(y, 1);
    autil_bigint_dump(y);
    autil_bigint_magnitude_shiftr(y, 1);
    autil_bigint_dump(y);

    autil_bigint_del(x);
    autil_bigint_del(y);
    return EXIT_SUCCESS;
}
