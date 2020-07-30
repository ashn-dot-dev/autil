#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct map* const m = map_new(sizeof(char const*), sizeof(int), str_vpcmp);
    int const A = 0x1;
    int const B = 0x3;
    int const C = 0x5;
    char const* oldkey = NULL;
    int oldval = 0;
    int existed = 0;

    // ["A"] -> A
    existed = map_insert(m, LOCAL_PTR(char const*, "A"), &A, NULL, NULL);
    ASSERT(existed == 0);
    ASSERT(map_count(m) == 1);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "A")) == A);

    // ["A"] -> 314
    existed = map_insert(
        m, LOCAL_PTR(char const*, "A"), LOCAL_PTR(int, 314), NULL, NULL);
    ASSERT(existed == 1);
    ASSERT(map_count(m) == 1);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "A")) == 314);

    // ["A"] -> 42
    existed = map_insert(
        m, LOCAL_PTR(char const*, "A"), LOCAL_PTR(int, 42), &oldkey, &oldval);
    ASSERT(existed == 1);
    ASSERT(map_count(m) == 1);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "A")) == 42);
    ASSERT(strcmp(oldkey, "A") == 0);
    ASSERT(oldval = 314);

    // ["A"] -> 42
    // ["B"] -> B
    existed = map_insert(
        m, LOCAL_PTR(char const*, "B"), LOCAL_PTR(int, B), NULL, NULL);
    ASSERT(existed == 0);
    ASSERT(map_count(m) == 2);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "A")) == 42);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "B")) == B);

    // ["A"] -> 42
    // ["B"] -> B
    // ["C"] -> C
    char const* str_C = "C";
    existed = map_insert(m, &str_C, &C, NULL, NULL);
    ASSERT(existed == 0);
    ASSERT(map_count(m) == 3);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "A")) == 42);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "B")) == B);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "C")) == C);

    // ["A"]   -> 42
    // ["B"]   -> B
    // ["C"]   -> C
    // ["!#$"] -> 99
    existed = map_insert(
        m, LOCAL_PTR(char const*, "!#$"), LOCAL_PTR(int, 99), NULL, NULL);
    EXPECT(map_count(m) == 4);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "A")) == 42);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "B")) == B);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "C")) == C);
    ASSERT(*(int*)map_lookup(m, LOCAL_PTR(char const*, "!#$")) == 99);

    map_del(m);
}
