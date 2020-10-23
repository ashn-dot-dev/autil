#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    for (int i = 0; i < 256; ++i) {
        printf("%c", to_upper(i));
    }
    fputc('\n', stdout);
    return EXIT_SUCCESS;
}
