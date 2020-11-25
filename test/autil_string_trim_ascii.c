#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string* s = NULL;

    s = autil_string_new_cstr(" \n\r\t\vA B C\v\t\r\n ");
    autil_string_trim_ascii(s);
    EXPECT(strcmp(autil_string_start(s), "A B C") == 0);
    autil_string_del(s);

    s = autil_string_new_cstr("");
    autil_string_trim_ascii(s);
    EXPECT(strcmp(autil_string_start(s), "") == 0);
    autil_string_del(s);

    s = autil_string_new_cstr("A B C");
    autil_string_trim_ascii(s);
    EXPECT(strcmp(autil_string_start(s), "A B C") == 0);
    autil_string_del(s);

    s = autil_string_new_cstr("   A B C");
    autil_string_trim_ascii(s);
    EXPECT(strcmp(autil_string_start(s), "A B C") == 0);
    autil_string_del(s);

    s = autil_string_new_cstr("A B C   ");
    autil_string_trim_ascii(s);
    EXPECT(strcmp(autil_string_start(s), "A B C") == 0);
    autil_string_del(s);
}
