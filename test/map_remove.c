#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct map* const m =
        map_new(sizeof(char const*), sizeof(char const*), str_vpcmp);
    char const* oldkey = NULL;
    char const* oldval = 0;
    int existed = 0;

    map_insert(
        m,
        LOCAL_PTR(char const*, "foo"),
        LOCAL_PTR(char const*, "FOO"),
        NULL,
        NULL);
    map_insert(
        m,
        LOCAL_PTR(char const*, "bar"),
        LOCAL_PTR(char const*, "BAR"),
        NULL,
        NULL);

    existed = map_remove(m, LOCAL_PTR(char const*, "foo"), NULL, NULL);
    ASSERT(existed == 1);
    ASSERT(map_count(m) == 1);

    existed = map_remove(m, LOCAL_PTR(char const*, "foo"), NULL, NULL);
    ASSERT(existed == 0);
    ASSERT(map_count(m) == 1);

    existed = map_remove(m, LOCAL_PTR(char const*, "bar"), &oldkey, &oldval);
    ASSERT(existed == 1);
    ASSERT(map_count(m) == 0);
    ASSERT(strcmp(oldkey, "bar") == 0);
    ASSERT(strcmp(oldval, "BAR") == 0);

    existed = map_remove(m, LOCAL_PTR(char const*, "bar"), &oldkey, &oldval);
    ASSERT(existed == 0);
    ASSERT(map_count(m) == 0);

    map_del(m);
}
