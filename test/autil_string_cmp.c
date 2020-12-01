#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_string* lhs = NULL;
    struct autil_string* rhs = NULL;

    lhs = autil_string_new(NULL, 0);
    rhs = autil_string_new(NULL, 0);
    EXPECT(autil_string_cmp(lhs, rhs) == 0);
    autil_string_del(lhs);
    autil_string_del(rhs);

    lhs = autil_string_new_cstr("A");
    rhs = autil_string_new_cstr("A");
    EXPECT(autil_string_cmp(lhs, rhs) == 0);
    autil_string_del(lhs);
    autil_string_del(rhs);

    lhs = autil_string_new_cstr("A");
    rhs = autil_string_new_cstr("B");
    EXPECT(autil_string_cmp(lhs, rhs) < 0);
    autil_string_del(lhs);
    autil_string_del(rhs);

    lhs = autil_string_new_cstr("B");
    rhs = autil_string_new_cstr("A");
    EXPECT(autil_string_cmp(lhs, rhs) > 0);
    autil_string_del(lhs);
    autil_string_del(rhs);

    lhs = autil_string_new_cstr("FooBar");
    rhs = autil_string_new("Fo\0Bar", 6);
    EXPECT(autil_string_cmp(lhs, rhs) > 0);
    autil_string_del(lhs);
    autil_string_del(rhs);

    lhs = autil_string_new_cstr("Fo\0\0ar");
    rhs = autil_string_new("Fo\0Bar", 6);
    EXPECT(autil_string_cmp(lhs, rhs) < 0);
    autil_string_del(lhs);
    autil_string_del(rhs);
}
