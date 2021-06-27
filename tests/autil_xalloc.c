#define AUTIL_IMPLEMENTATION
#include "../autil.h"

#if defined(__has_feature)
#    if __has_feature(address_sanitizer)
#        define AUTIL_ASAN_ENABLED
#    endif
#elif defined(__GNUC__) && defined(__SANITIZE_ADDRESS__)
#    define AUTIL_ASAN_ENABLED
#endif

int
main(void)
{
    size_t const size = 4096;
    char* const mem = autil_xalloc(NULL, size);
    for (size_t i = 0; i < size; ++i) {
        // Set each byte to some value just to make source touching that address
        // doesn't cause a segfault.
        mem[i] = 'A';
    }
    // Free memory.
    autil_xalloc(mem, AUTIL_XALLOC_FREE);

#ifndef AUTIL_ASAN_ENABLED
    // Crash the program by asking for more memory than the system could ever
    // provide from a single allocation, thus causing realloc to return NULL
    // and triggering an out-of-memory condition in autil_xalloc.
    // This behavior has been tested with the realloc implementation in glibc
    // and musl libc.
    autil_xalloc(NULL, (size_t)-1);
#else
    puts("error: [autil_xalloc] Out of memory");
    exit(EXIT_FAILURE);
#endif

    puts("UNREACHABLE");
    return EXIT_SUCCESS;
}
