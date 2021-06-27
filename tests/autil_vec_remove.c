#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_vec* const v = autil_vec_new(sizeof(int));
    int const A = 0x1;
    int const B = 0x3;
    int const C = 0x5;
    int const D = 0x7;
    int const E = 0x9;

    // [A][B][C][D][E]
    autil_vec_insert(v, 0, &A);
    autil_vec_insert(v, 1, &B);
    autil_vec_insert(v, 2, &C);
    autil_vec_insert(v, 3, &D);
    autil_vec_insert(v, 4, &E);
    ASSERT(autil_vec_count(v) == 5);
    ASSERT(*(int*)autil_vec_ref(v, 0) == A);
    ASSERT(*(int*)autil_vec_ref(v, 1) == B);
    ASSERT(*(int*)autil_vec_ref(v, 2) == C);
    ASSERT(*(int*)autil_vec_ref(v, 3) == D);
    ASSERT(*(int*)autil_vec_ref(v, 4) == E);

    // [A][B][C][D]
    autil_vec_remove(v, 4, NULL);
    ASSERT(autil_vec_count(v) == 4);
    ASSERT(*(int*)autil_vec_ref(v, 0) == A);
    ASSERT(*(int*)autil_vec_ref(v, 1) == B);
    ASSERT(*(int*)autil_vec_ref(v, 2) == C);
    ASSERT(*(int*)autil_vec_ref(v, 3) == D);

    // [A][B][D]
    autil_vec_remove(v, 2, NULL);
    ASSERT(autil_vec_count(v) == 3);
    ASSERT(*(int*)autil_vec_ref(v, 0) == A);
    ASSERT(*(int*)autil_vec_ref(v, 1) == B);
    ASSERT(*(int*)autil_vec_ref(v, 2) == D);

    // [B][D]
    autil_vec_remove(v, 0, NULL);
    ASSERT(autil_vec_count(v) == 2);
    ASSERT(*(int*)autil_vec_ref(v, 0) == B);
    ASSERT(*(int*)autil_vec_ref(v, 1) == D);

    // [B][D][42]
    autil_vec_insert(v, 2, (int[]){42});
    int removed = 0;
    // [B][D]
    // removed := 42
    autil_vec_remove(v, 2, &removed);
    ASSERT(removed == 42);

    //  0  1  2
    // [B][D][!]
    //        ^
    //        + Element does no exist. Removing here should cause an error.
    autil_vec_remove(v, 2, NULL);

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
