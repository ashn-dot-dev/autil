#define AUTIL_IMPLEMENTATION
#include "../autil.h"

#define FOO "foo!"

int
main(void)
{
    printf("%zu\n", AUTIL_CSTR_COUNT(FOO));
    return EXIT_SUCCESS;
}
