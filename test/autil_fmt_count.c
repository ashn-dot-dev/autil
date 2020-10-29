#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    printf("%zu\n", AUTIL_FMT_COUNT("%d", 1));
    printf("%zu\n", AUTIL_FMT_COUNT("%d", 11));
    printf("%zu\n", AUTIL_FMT_COUNT("%d", 111));

    fputc('\n', stdout);

    printf("%zu\n", AUTIL_FMT_COUNT("%s", "foo"));
    printf("%zu\n", AUTIL_FMT_COUNT("%.*s", 2, "foo"));

    return EXIT_SUCCESS;
}

