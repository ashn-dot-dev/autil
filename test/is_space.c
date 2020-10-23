#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    for (int i = 0; i < 256; ++i) {
        printf("%c", is_space(i) ? '1' : '0');
    }
    fputc('\n', stdout);
    return EXIT_SUCCESS;
}
