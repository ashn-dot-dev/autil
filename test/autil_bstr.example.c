#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

static struct autil_bstr const static_bstr = {"foo", AUTIL_CSTR_COUNT("foo")};

int
main(void)
{
    printf(
        "static literal \"%.*s\": %zu bytes\n",
        (int)static_bstr.count,
        static_bstr.start,
        static_bstr.count);

    struct autil_bstr const local_bstr = AUTIL_BSTR_LITERAL("bar");
    printf(
        "local literal \"%.*s\": %zu bytes\n",
        (int)local_bstr.count,
        local_bstr.start,
        local_bstr.count);

    char const* const str = "the string foobar";
    struct autil_bstr init = {&str[11], 6};
    printf("\"%.*s\": %zu bytes\n", (int)init.count, init.start, init.count);

    return EXIT_SUCCESS;
}
