#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_map* const m =
        autil_map_new(sizeof(int), sizeof(int), autil_int_vpcmp);

    ASSERT(NULL == autil_map_lookup(m, AUTIL_LOCAL_PTR(int, 42)));

    autil_map_insert(
        m, AUTIL_LOCAL_PTR(int, 42), AUTIL_LOCAL_PTR(int, 0x01), NULL, NULL);
    autil_map_insert(
        m, AUTIL_LOCAL_PTR(int, 41), AUTIL_LOCAL_PTR(int, 0x03), NULL, NULL);
    autil_map_insert(
        m, AUTIL_LOCAL_PTR(int, 43), AUTIL_LOCAL_PTR(int, 0x05), NULL, NULL);

    ASSERT(0x01 == *(int*)autil_map_lookup(m, AUTIL_LOCAL_PTR(int, 42)));
    ASSERT(0x03 == *(int*)autil_map_lookup(m, AUTIL_LOCAL_PTR(int, 41)));
    ASSERT(0x05 == *(int*)autil_map_lookup(m, AUTIL_LOCAL_PTR(int, 43)));

    struct autil_map const* const ref = m;
    ASSERT(
        AUTIL_DEREF_PTR(
            int, autil_map_lookup_const(ref, AUTIL_LOCAL_PTR(int, 42)))
        == 0x01);

    autil_map_del(m);
}
