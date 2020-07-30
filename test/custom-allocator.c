#include <stddef.h>
void*
custom_realloc(void* ptr, size_t size);
void
custom_free(void* ptr);

#define AUTIL_IMPLEMENTATION
#define AUTIL_REALLOC custom_realloc
#define AUTIL_FREE custom_free
#include "../autil.h"

int
main(void)
{
    xalloc(xalloc(NULL, 4096), XALLOC_FREE);
    return EXIT_SUCCESS;
}

void*
custom_realloc(void* ptr, size_t size)
{
    puts("CUSTOM REALLOC");
    return realloc(ptr, size);
}

void
custom_free(void* ptr)
{
    puts("CUSTOM FREE");
    free(ptr);
}
