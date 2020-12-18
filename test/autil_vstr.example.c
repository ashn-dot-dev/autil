#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

static struct autil_vstr const static_vstr = AUTIL_VSTR_LITERAL_INIT("foo");

int
main(void)
{
    printf(
        "static literal \"%.*s\": %zu bytes\n",
        (int)static_vstr.count,
        static_vstr.start,
        static_vstr.count);

    struct autil_vstr const local_vstr = AUTIL_VSTR_LITERAL("bar");
    printf(
        "local literal \"%.*s\": %zu bytes\n",
        (int)local_vstr.count,
        local_vstr.start,
        local_vstr.count);

    char const* const str = "the string foobar";
    struct autil_vstr init = {&str[11], 6};
    printf("\"%.*s\": %zu bytes\n", (int)init.count, init.start, init.count);

    return EXIT_SUCCESS;
}
