#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bitarr* a = NULL;

    a = autil_bitarr_new(0);
    ASSERT(autil_bitarr_count(a) == 0);
    autil_bitarr_del(a);

    a = autil_bitarr_new(1);
    ASSERT(autil_bitarr_count(a) == 1);
    autil_bitarr_del(a);

    a = autil_bitarr_new(2);
    ASSERT(autil_bitarr_count(a) == 2);
    autil_bitarr_del(a);

    a = autil_bitarr_new(256);
    ASSERT(autil_bitarr_count(a) == 256);
    autil_bitarr_del(a);

    a = autil_bitarr_new(4096);
    ASSERT(autil_bitarr_count(a) == 4096);
    autil_bitarr_del(a);
}
