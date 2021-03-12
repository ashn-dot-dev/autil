#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bitarr* const a = autil_bitarr_new(4096);

    // autil_bitarr_set is exercised by autil_bitarr_get.c
    // This just tests index out of bounds.
    autil_bitarr_set(a, 4096u, 1);

    puts("UNREACHABLE");
    autil_bitarr_del(a);
    return EXIT_SUCCESS;
}
