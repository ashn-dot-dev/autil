#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string* const s = autil_string_new_cstr("abc");
    ASSERT(autil_string_count(s) == 3);

    autil_string_resize(s, 6);
    EXPECT(autil_string_count(s) == 6);
    EXPECT(strcmp(autil_string_start(s), "abc") == 0);
    *autil_string_ref(s, 3) = 'A';
    EXPECT(strcmp(autil_string_start(s), "abcA") == 0);
    *autil_string_ref(s, 5) = 'C';
    EXPECT(strcmp(autil_string_start(s), "abcA") == 0);
    *autil_string_ref(s, 4) = 'B';
    EXPECT(strcmp(autil_string_start(s), "abcABC") == 0);

    autil_string_resize(s, 3);
    EXPECT(strcmp(autil_string_start(s), "abc") == 0);

    autil_string_del(s);
}
