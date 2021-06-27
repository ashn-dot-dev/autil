#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    size_t const nmemb = 4096;
    int* const mem = autil_xallocn(NULL, nmemb, sizeof(int));
    for (size_t i = 0; i < nmemb; ++i) {
        // Set each element to some value just to make source touching that
        // address doesn't cause a segfault.
        mem[i] = (int)i;
    }
    // Free memory.
    autil_xallocn(mem, 0, AUTIL_XALLOC_FREE);

    // Crash the program by asking a total amount of memory that would overflow
    // when nmemb and size are multiplied together.
    autil_xallocn(NULL, (size_t)-1, sizeof(long));

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
