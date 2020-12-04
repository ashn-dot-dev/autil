#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    struct autil_map* const m =
        autil_map_new(sizeof(char const*), sizeof(int), autil_str_vpcmp);
    char const* const keyfoo = "foo";
    char const* const keybar = "bar";
    char const* oldkey;
    int oldval;
    int val;

    val = 3;
    autil_map_insert(m, &keyfoo, &val, NULL, NULL);

    val = 5;
    autil_map_insert(m, &keybar, &val, NULL, NULL);

    printf("%d\n", *(int*)autil_map_lookup(m, &keyfoo)); // 3
    printf("%d\n", *(int*)autil_map_lookup(m, &keybar)); // 5

    val = 42;
    autil_map_insert(m, &keyfoo, &val, NULL, &oldval);

    printf("%d\n", *(int*)autil_map_lookup(m, &keyfoo)); // 42
    printf("%d\n", *(int*)autil_map_lookup(m, &keybar)); // 5
    printf("%d\n", oldval); // 3

    autil_map_remove(m, &keyfoo, &oldkey, &oldval);
    printf("REMOVED: \"%s\", %d\n", oldkey, oldval); // REMOVED: "foo", 42

    autil_map_del(m);
    return EXIT_SUCCESS;
}
