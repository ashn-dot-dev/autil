#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_sipool* const pool = autil_sipool_new();

    char const str1[] = {'f', 'o', 'o'};
    char const* const str2 = "foo";
    char* const str3 = autil_cstr_new_cstr("foo");
    char const* const str4 = "fo";

    char const* const intern1 = autil_sipool_intern(pool, str1, 3);
    char const* const intern2 = autil_sipool_intern_cstr(pool, str2);
    char const* const intern3 = autil_sipool_intern_cstr(pool, str3);
    char const* const intern4 = autil_sipool_intern_cstr(pool, str4);
    char const* const intern5 = autil_sipool_intern(pool, "foobar", 2);
    char const* const intern6 = autil_sipool_intern(pool, NULL, 0);
    char const* const intern7 = autil_sipool_intern_cstr(pool, "");

    ASSERT(intern1 == intern2);
    ASSERT(intern1 == intern3);
    ASSERT(intern1 != intern4);
    ASSERT(intern4 == intern5);
    ASSERT(intern6 == intern7);

    autil_xalloc(str3, AUTIL_XALLOC_FREE);
    autil_sipool_del(pool);
}
