#define AUTIL_IMPLEMENTATION
#include "../autil.h"

#define FOO "foo!"

int
main(void)
{
    printf("%zu\n", AUTIL_STR_LITERAL_COUNT(FOO));
    return EXIT_SUCCESS;
}
