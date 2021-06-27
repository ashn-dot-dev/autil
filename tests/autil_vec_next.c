#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

static void
test_empty(void)
{
    printf("%s\n", __func__);
    struct autil_vec* const v = autil_vec_new(sizeof(int));

    int const* iterm = autil_vec_next(v, NULL);
    ASSERT(iterm == NULL);
    int const* iterc = autil_vec_next_const(v, NULL);
    ASSERT(iterc == NULL);

    autil_vec_del(v);
}

static void
test_zero_sized_elements(void)
{
    printf("%s\n", __func__);
    struct autil_vec* const v = autil_vec_new(0);
    autil_vec_insert(v, 0, NULL);
    autil_vec_insert(v, 0, NULL);
    autil_vec_insert(v, 0, NULL);
    ASSERT(autil_vec_count(v) == 3);

    int const* iterm = autil_vec_next(v, NULL);
    ASSERT(iterm == NULL);
    int const* iterc = autil_vec_next_const(v, NULL);
    ASSERT(iterc == NULL);

    autil_vec_del(v);
}

static void
test_mutable(void)
{
    printf("%s\n", __func__);
    struct autil_vec* const v = autil_vec_new(sizeof(int));
    autil_vec_insert(v, autil_vec_count(v), (int[]){1});
    autil_vec_insert(v, autil_vec_count(v), (int[]){2});
    autil_vec_insert(v, autil_vec_count(v), (int[]){3});
    autil_vec_insert(v, autil_vec_count(v), (int[]){4});
    autil_vec_insert(v, autil_vec_count(v), (int[]){5});

    int* iter = autil_vec_next(v, NULL);
    for (; iter != NULL; iter = autil_vec_next(v, iter)) {
        *iter += 10;
        printf("%d\n", *iter);
    }

    autil_vec_del(v);
}

static void
test_const(void)
{
    printf("%s\n", __func__);
    struct autil_vec* const v = autil_vec_new(sizeof(int));
    autil_vec_insert(v, autil_vec_count(v), (int[]){1});
    autil_vec_insert(v, autil_vec_count(v), (int[]){2});
    autil_vec_insert(v, autil_vec_count(v), (int[]){3});
    autil_vec_insert(v, autil_vec_count(v), (int[]){4});
    autil_vec_insert(v, autil_vec_count(v), (int[]){5});

    int const* iter = autil_vec_next_const(v, NULL);
    for (; iter != NULL; iter = autil_vec_next_const(v, iter)) {
        printf("%d\n", *iter);
    }

    autil_vec_del(v);
}

int
main(void)
{
    test_empty();
    test_zero_sized_elements();
    test_mutable();
    test_const();
    return EXIT_SUCCESS;
}
