#define AUTIL_IMPLEMENTATION
#define AUTIL_API static

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#include "../autil.h"

int
main(void)
{
    puts("AUTIL COMPILED WITH STATIC LINKAGE");
    return EXIT_SUCCESS;
}
