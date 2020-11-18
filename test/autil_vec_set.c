#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    int const A = 0x1;
    int const B = 0x3;

    // [A][A][A]
    struct autil_vec* const v = autil_vec_new(sizeof(int));
    autil_vec_insert(v, 0, &A);
    autil_vec_insert(v, 1, &A);
    autil_vec_insert(v, 2, &A);

    autil_vec_set(v, 0, &B);
    ASSERT(*(int*)autil_vec_ref(v, 0) == B);

    autil_vec_set(v, 1, AUTIL_LOCAL_PTR(int, 42));
    ASSERT(*(int*)autil_vec_ref(v, 1) == 42);

    struct
    {
        int val;
    } C;
    C.val = 5;
    autil_vec_set(v, 2, AUTIL_LOCAL_PTR(int, C.val + 1));
    ASSERT(*(int*)autil_vec_ref(v, 2) == 6);

    //  0  1  2  3
    // [B][B][B][!]
    //           ^
    //           + Element does no exist.
    autil_vec_set(v, 3, &B);

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
