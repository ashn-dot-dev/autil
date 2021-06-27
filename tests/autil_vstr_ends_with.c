#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    EXPECT(autil_vstr_ends_with(
        AUTIL_VSTR_LOCAL_PTR("", 0), AUTIL_VSTR_LOCAL_PTR("", 0)));
    EXPECT(autil_vstr_ends_with(
        AUTIL_VSTR_LOCAL_PTR("FOOBAR", 6), AUTIL_VSTR_LOCAL_PTR("", 0)));
    EXPECT(autil_vstr_ends_with(
        AUTIL_VSTR_LOCAL_PTR("FOOBAR", 3), AUTIL_VSTR_LOCAL_PTR("", 0)));

    EXPECT(autil_vstr_ends_with(
        AUTIL_VSTR_LOCAL_PTR_STR_LITERAL("FOOBAR"),
        AUTIL_VSTR_LOCAL_PTR_STR_LITERAL("BAR")));
    EXPECT(!autil_vstr_ends_with(
        AUTIL_VSTR_LOCAL_PTR_STR_LITERAL("FOOBAR"),
        AUTIL_VSTR_LOCAL_PTR_STR_LITERAL("FOO")));
    EXPECT(!autil_vstr_ends_with(
        AUTIL_VSTR_LOCAL_PTR_STR_LITERAL("FOO"),
        AUTIL_VSTR_LOCAL_PTR_STR_LITERAL("FOOBAR")));
    EXPECT(!autil_vstr_ends_with(
        AUTIL_VSTR_LOCAL_PTR_STR_LITERAL("FOO"),
        AUTIL_VSTR_LOCAL_PTR_STR_LITERAL("FOOFOO")));

    EXPECT(autil_vstr_ends_with(
        AUTIL_VSTR_LOCAL_PTR_STR_LITERAL("\0FOO\0BAR"),
        AUTIL_VSTR_LOCAL_PTR_STR_LITERAL("\0BAR")));
}
