#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct vec* const v = vec_new(sizeof(int));

    vec_resize(v, 10);
    EXPECT(vec_count(v) == 10);
    EXPECT(vec_capacity(v) >= 10);

    vec_resize(v, 100);
    EXPECT(vec_count(v) == 100);
    EXPECT(vec_capacity(v) >= 100);

    vec_resize(v, 100000);
    EXPECT(vec_count(v) == 100000);
    EXPECT(vec_capacity(v) >= 100000);

    vec_del(v);

    return EXIT_SUCCESS;
}