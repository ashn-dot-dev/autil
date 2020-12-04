#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int some_int = 42;

int
main(void)
{
    struct autil_vec* const v = autil_vec_new(0);
    ASSERT(autil_vec_elemsize(v) == 0);

    autil_vec_insert(v, 0, NULL);
    autil_vec_insert(v, 1, &some_int);
    ASSERT(autil_vec_count(v) == 2);
    ASSERT(autil_vec_ref(v, 0) == NULL);
    ASSERT(autil_vec_ref(v, 1) == NULL);

    some_int = 314;

    autil_vec_remove(v, 1, &some_int);
    autil_vec_remove(v, 0, NULL);
    ASSERT(autil_vec_count(v) == 0);
    ASSERT(some_int == 314);

    autil_vec_del(v);
    return EXIT_SUCCESS;
}
