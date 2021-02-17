#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    struct autil_bigint* x = autil_bigint_new_cstr("0xff0000");
    autil_bigint_dump(x);

    autil_bigint_shiftr(x, 0);
    autil_bigint_dump(x);

    autil_bigint_shiftr(x, 4);
    autil_bigint_dump(x);

    autil_bigint_shiftr(x, 7);
    autil_bigint_dump(x);

    autil_bigint_shiftr(x, 1);
    autil_bigint_dump(x);

    fflush(stdout);
    autil_bigint_shiftr(x, 32);
    autil_bigint_dump(x);

    autil_bigint_del(x);
    return EXIT_SUCCESS;
}