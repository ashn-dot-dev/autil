#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    int const A = 0x1;
    int const B = 0x3;

    // [A][A][A]
    struct vec* const v = vec_new(sizeof(int));
    vec_insert(v, 0, &A);
    vec_insert(v, 1, &A);
    vec_insert(v, 2, &A);

    vec_set(v, 0, &B);
    ASSERT(*(int*)vec_get(v, 0) == B);

    vec_set(v, 1, LOCAL_PTR(int, 42));
    ASSERT(*(int*)vec_get(v, 1) == 42);

    struct
    {
        int val;
    } C;
    C.val = 5;
    vec_set(v, 2, LOCAL_PTR(int, C.val + 1));
    ASSERT(*(int*)vec_get(v, 2) == 6);

    //  0  1  2  3
    // [B][B][B][!]
    //           ^
    //           + Element does no exist.
    vec_set(v, 3, &B);

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
