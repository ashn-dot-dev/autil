#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int arr3[] = {1, 2, 3};
int arr5[] = {1, 2, 3, 4, 5};
enum
{
    COMPILE_TIME_CONSTANT = AUTIL_ARRAY_COUNT(arr3)
};

int
main(void)
{
    printf("%zu %zu\n", AUTIL_ARRAY_COUNT(arr3), AUTIL_ARRAY_COUNT(arr5));
    return EXIT_SUCCESS;
}
