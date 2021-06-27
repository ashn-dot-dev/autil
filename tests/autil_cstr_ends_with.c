#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    EXPECT(autil_cstr_ends_with("", ""));
    EXPECT(autil_cstr_ends_with("FOOBAR", ""));
    EXPECT(autil_cstr_ends_with("FOOBAR", "FOOBAR"));

    EXPECT(autil_cstr_ends_with("FOOBAR", "BAR"));
    EXPECT(!autil_cstr_ends_with("FOOBAR", "FOO"));
    EXPECT(!autil_cstr_ends_with("FOO", "FOOBAR"));
    EXPECT(!autil_cstr_ends_with("FOO", "FOOFOO"));

    EXPECT(autil_cstr_ends_with("\0FOO\0BAR", "\0FOO\0B"));
}
