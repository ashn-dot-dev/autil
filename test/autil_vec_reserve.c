#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_vec* const v = autil_vec_new(sizeof(int));

    autil_vec_reserve(v, 10);
    EXPECT(autil_vec_count(v) == 0);
    EXPECT(autil_vec_capacity(v) >= 10);

    autil_vec_reserve(v, 100);
    EXPECT(autil_vec_count(v) == 0);
    EXPECT(autil_vec_capacity(v) >= 100);

    autil_vec_reserve(v, 100000);
    EXPECT(autil_vec_count(v) == 0);
    EXPECT(autil_vec_capacity(v) >= 100000);

    autil_vec_del(v);

    return EXIT_SUCCESS;
}
