#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    struct autil_freezer* f = autil_freezer_new();

    autil_freezer_register(f, NULL);
    autil_freezer_register(f, autil_xalloc(NULL, 123u));
    autil_freezer_register(f, autil_xalloc(NULL, 0xFFFFu));

    struct autil_bigint* const bigint = autil_bigint_new_cstr("0x123");
    autil_bigint_freeze(bigint, f);

    autil_freezer_del(f);
}
