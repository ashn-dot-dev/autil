#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct map* const m = map_new(sizeof(int), sizeof(int), int_vpcmp);

    ASSERT(NULL == map_lookup(m, LOCAL_PTR(int, 42)));

    map_insert(m, LOCAL_PTR(int, 42), LOCAL_PTR(int, 0x01), NULL, NULL);
    map_insert(m, LOCAL_PTR(int, 41), LOCAL_PTR(int, 0x03), NULL, NULL);
    map_insert(m, LOCAL_PTR(int, 43), LOCAL_PTR(int, 0x05), NULL, NULL);

    ASSERT(0x01 == *(int*)map_lookup(m, LOCAL_PTR(int, 42)));
    ASSERT(0x03 == *(int*)map_lookup(m, LOCAL_PTR(int, 41)));
    ASSERT(0x05 == *(int*)map_lookup(m, LOCAL_PTR(int, 43)));

    map_del(m);
}
