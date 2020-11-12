#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    void* data = NULL;
    size_t size = 0;

    autil_xalloc_append(&data, &size, NULL, 0);
    autil_xalloc_append(&data, &size, "FOOBAR", 0);
    autil_xalloc_append(&data, &size, "A", 1);
    autil_xalloc_append(&data, &size, "BB", 1);
    autil_xalloc_append(&data, &size, "CC", 2);

    printf("%zu\n", size);
    printf("%.*s\n", (int)size, (char const*)data);

    autil_xalloc(data, AUTIL_XALLOC_FREE);
}
