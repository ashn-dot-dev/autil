#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    // Initialize a zero element arr by assigning it NULL.
    int* a = NULL;
    ASSERT(AUTIL_ARR_COUNT(a) == 0);
    ASSERT(AUTIL_ARR_CAPACITY(a) == 0);

    // Increase the number of elements in the arr.
    size_t count = 12;
    AUTIL_ARR_RESIZE(a, count);
    ASSERT(a != NULL);
    ASSERT(AUTIL_ARR_COUNT(a) == count);
    ASSERT(AUTIL_ARR_CAPACITY(a) >= count);
    // Assign values...
    for (size_t i = 0; i < count; ++i) {
        a[i] = (int)i + 1;
    }
    // And print them out...
    for (size_t i = 0; i < count; ++i) {
        printf("%d\n", a[i]);
    }

    // We can increase the capacity to store more values in the future without
    // requiring reallocation while still keeping the current count.
    AUTIL_ARR_RESERVE(a, count + 1000);
    ASSERT(AUTIL_ARR_COUNT(a) == count);
    ASSERT(AUTIL_ARR_CAPACITY(a) >= count + 1000);

    // Reset the count to zero and resize the arr to fit zero elements.
    count = 0;
    AUTIL_ARR_RESIZE(a, count);
    ASSERT(a == NULL);
    ASSERT(AUTIL_ARR_COUNT(a) == count);
    ASSERT(AUTIL_ARR_CAPACITY(a) >= count);

    // Push values onto the back of the arr.
    count = 10000;
    for (size_t i = 0; i < count; ++i) {
        AUTIL_ARR_PUSH(a, (int)i);
    }
    ASSERT(a != NULL);
    ASSERT(AUTIL_ARR_COUNT(a) == count);
    ASSERT(AUTIL_ARR_CAPACITY(a) >= count);
    for (size_t i = 0; i < count; ++i) {
        ASSERT(a[i] == (int)i);
    }

    // Pop a couple of values off the back of the arr.
    ASSERT(AUTIL_ARR_POP(a) == (int)count - 1);
    ASSERT(AUTIL_ARR_POP(a) == (int)count - 2);
    ASSERT(AUTIL_ARR_COUNT(a) == count - 2);

    AUTIL_ARR_FINI(a);
    return EXIT_SUCCESS;
}
