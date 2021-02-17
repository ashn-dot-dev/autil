#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    struct autil_bigint* x = autil_bigint_new_cstr("0xff");
    autil_bigint_dump(x);

    autil_bigint_shiftl(x, 0);
    autil_bigint_dump(x);

    autil_bigint_shiftl(x, 1);
    autil_bigint_dump(x);

    autil_bigint_shiftl(x, 7);
    autil_bigint_dump(x);

    autil_bigint_shiftl(x, 4);
    autil_bigint_dump(x);

    struct autil_bigint* const y = autil_bigint_new_cstr("-0b1");
    autil_bigint_dump(y);
    autil_bigint_shiftl(y, 1);
    autil_bigint_dump(y);

    struct autil_bigint* const z = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_shiftl(z, 1);
    autil_bigint_dump(z);

    autil_bigint_del(x);
    autil_bigint_del(y);
    autil_bigint_del(z);
    return EXIT_SUCCESS;
}
