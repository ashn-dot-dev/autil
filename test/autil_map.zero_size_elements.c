#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_map* const m = autil_map_new(0, 0, autil_void_vpcmp);

    autil_map_insert(m, NULL, NULL, NULL, NULL);
    ASSERT(autil_map_count(m) == 1);
    autil_map_insert(m, NULL, NULL, NULL, NULL);
    ASSERT(autil_map_count(m) == 1); // Should still stay 1

    int foo = 1;
    char bar = 'b';
    autil_map_insert(m, &foo, &bar, NULL, NULL);
    ASSERT(autil_map_count(m) == 1);

    autil_map_del(m);
    return EXIT_SUCCESS;
}
