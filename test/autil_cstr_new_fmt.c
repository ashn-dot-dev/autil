#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{

    char* const str0 = autil_cstr_new_fmt("");
    char* const str1 = autil_cstr_new_fmt("foo");
    char* const str2 = autil_cstr_new_fmt("foo\0bar");
    char* const str3 = autil_cstr_new_fmt("%d %s %c", 123, "foo", 'c');

    ASSERT(strcmp(str0, "") == 0);
    ASSERT(strcmp(str1, "foo") == 0);
    ASSERT(strcmp(str2, "foo") == 0);
    ASSERT(strcmp(str3, "123 foo c") == 0);

    autil_xalloc(str0, AUTIL_XALLOC_FREE);
    autil_xalloc(str1, AUTIL_XALLOC_FREE);
    autil_xalloc(str2, AUTIL_XALLOC_FREE);
    autil_xalloc(str3, AUTIL_XALLOC_FREE);

    return EXIT_SUCCESS;
}

