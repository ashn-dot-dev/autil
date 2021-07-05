#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    struct autil_freezer* f = autil_freezer_new();
    autil_freezer_register(f, NULL);
    autil_freezer_register(f, autil_xalloc(NULL, 123u));
    autil_freezer_register(f, autil_xalloc(NULL, 0xFFFFu));
    autil_freezer_del(f);
}
