#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_map* const m = autil_map_new(
        sizeof(char const*), sizeof(char const*), autil_cstr_vpcmp);
    char const* oldkey = NULL;
    char const* oldval = 0;
    int existed = 0;

    autil_map_insert(
        m,
        AUTIL_LOCAL_PTR(char const*, "foo"),
        AUTIL_LOCAL_PTR(char const*, "FOO"),
        NULL,
        NULL);
    autil_map_insert(
        m,
        AUTIL_LOCAL_PTR(char const*, "bar"),
        AUTIL_LOCAL_PTR(char const*, "BAR"),
        NULL,
        NULL);

    existed =
        autil_map_remove(m, AUTIL_LOCAL_PTR(char const*, "foo"), NULL, NULL);
    ASSERT(existed == 1);
    ASSERT(autil_map_count(m) == 1);

    existed =
        autil_map_remove(m, AUTIL_LOCAL_PTR(char const*, "foo"), NULL, NULL);
    ASSERT(existed == 0);
    ASSERT(autil_map_count(m) == 1);

    existed = autil_map_remove(
        m, AUTIL_LOCAL_PTR(char const*, "bar"), &oldkey, &oldval);
    ASSERT(existed == 1);
    ASSERT(autil_map_count(m) == 0);
    ASSERT(strcmp(oldkey, "bar") == 0);
    ASSERT(strcmp(oldval, "BAR") == 0);

    existed = autil_map_remove(
        m, AUTIL_LOCAL_PTR(char const*, "bar"), &oldkey, &oldval);
    ASSERT(existed == 0);
    ASSERT(autil_map_count(m) == 0);

    autil_map_del(m);
}
