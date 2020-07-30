#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct vec* const v = vec_new(sizeof(int));
    int const A = 0x1;
    int const B = 0x3;
    int const E = 0x9;

    // [A]
    vec_insert(v, 0, &A);
    ASSERT(vec_count(v) == 1);
    ASSERT(*(int*)vec_get(v, 0) == A);

    // [A][B]
    vec_insert(v, 1, &B);
    ASSERT(vec_count(v) == 2);
    ASSERT(*(int*)vec_get(v, 1) == B);

    // [C][A][B]
    struct
    {
        int val;
    } C;
    C.val = 4;
    vec_insert(v, 0, LOCAL_PTR(int, C.val + 1));
    ASSERT(vec_count(v) == 3);
    ASSERT(*(int*)vec_get(v, 0) == 0x5);

    // [C][D][A][B]
    // clang-format off
    vec_insert(
        v,
        1,
        LOCAL_PTR(
            int,
            ((struct {int val; char const* str;}){.val = 0x7, .str = "foo"}).val
        )
    );
    // clang-format on
    ASSERT(vec_count(v) == 4);
    ASSERT(*(int*)vec_get(v, 1) == 0x7);

    //  0  1  2  3  4  5
    // [C][D][A][B][!][E]
    //              ^  ^
    //              |  + Inserting here should cause an error.
    //              + Missing element.
    vec_insert(v, 5, &E);

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
