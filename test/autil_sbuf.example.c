#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

static void
basics(void)
{
    // Initialize a zero element arr by assigning it NULL.
    int* a = NULL;
    ASSERT(autil_sbuf_count(a) == 0);
    ASSERT(autil_sbuf_capacity(a) == 0);

    // Increase the number of elements in the arr.
    size_t count = 12;
    autil_sbuf_resize(a, count);
    ASSERT(a != NULL);
    ASSERT(autil_sbuf_count(a) == count);
    ASSERT(autil_sbuf_capacity(a) >= count);
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
    autil_sbuf_reserve(a, count + 1000);
    ASSERT(autil_sbuf_count(a) == count);
    ASSERT(autil_sbuf_capacity(a) >= count + 1000);

    // Reset the count to zero and resize the arr to fit zero elements.
    count = 0;
    autil_sbuf_resize(a, count);
    ASSERT(a == NULL);
    ASSERT(autil_sbuf_count(a) == count);
    ASSERT(autil_sbuf_capacity(a) >= count);

    // Push values onto the back of the arr.
    count = 10000;
    for (size_t i = 0; i < count; ++i) {
        autil_sbuf_push(a, (int)i);
    }
    ASSERT(a != NULL);
    ASSERT(autil_sbuf_count(a) == count);
    ASSERT(autil_sbuf_capacity(a) >= count);
    for (size_t i = 0; i < count; ++i) {
        ASSERT(a[i] == (int)i);
    }

    // Pop a couple of values off the back of the arr.
    ASSERT(autil_sbuf_pop(a) == (int)count - 1);
    ASSERT(autil_sbuf_pop(a) == (int)count - 2);
    ASSERT(autil_sbuf_count(a) == count - 2);

    autil_sbuf_fini(a);
}

struct point
{
    int x;
    int y;
};

static struct point
make_point(int x, int y)
{
    return (struct point){x, y};
}

static void
variadic_macros(void)
{
    struct point* a = NULL;

    // Despite being implemented as macros, the autil_sbuf_* functions accept
    // function calls and struct literals just fine.
    struct point foo = {1, 2};
    struct point bar = {.y = 4, .x = 3};
    autil_sbuf_push(a, foo);
    autil_sbuf_push(a, bar);
    autil_sbuf_push(a, (struct point){5, 6});
    autil_sbuf_push(a, (struct point){.y = 8, .x = 7});
    autil_sbuf_push(a, make_point(9, 10));

    for (size_t i = 0; i < autil_sbuf_count(a); ++i) {
        printf("x=%d y=%d\n", a[i].x, a[i].y);
    }

    // This is just silly. The point structure is reused just to show that
    // commas may appear in these macros.
    autil_sbuf_reserve(a, (size_t)(struct point){0, 20}.y);
    autil_sbuf_resize(a, (size_t)(struct point){0, 10}.y);

    autil_sbuf_fini(a);
}

// clang-format off
static void
mutate_ints(autil_sbuf(int) arr)
// clang-format on
{
    for (size_t i = 0; i < autil_sbuf_count(arr); ++i) {
        arr[i] += 10;
    }
}

// clang-format off
static void
print_ints(autil_sbuf_const(int) arr)
// clang-format on
{
    for (size_t i = 0; i < autil_sbuf_count(arr); ++i) {
        printf("%d\n", arr[i]);
    }
}

static void
alternative_declaration(void)
{
    autil_sbuf(int) arr = NULL;
    autil_sbuf_push(arr, 1);
    autil_sbuf_push(arr, 2);
    autil_sbuf_push(arr, 3);
    autil_sbuf_push(arr, 4);
    autil_sbuf_push(arr, 5);

    print_ints(arr);
    mutate_ints(arr);
    print_ints(arr);

    autil_sbuf_fini(arr);
}

int
main(void)
{
    basics();
    variadic_macros();
    alternative_declaration();
    return EXIT_SUCCESS;
}
