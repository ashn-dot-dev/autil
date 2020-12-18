#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_vstr lhs = {0};
    struct autil_vstr rhs = {0};

    int const local_ptr_cmp = autil_vstr_cmp(
        AUTIL_VSTR_LOCAL_PTR("foo", 3), AUTIL_VSTR_LOCAL_PTR("bar", 3));
    EXPECT(local_ptr_cmp != 0);

    lhs = (struct autil_vstr){NULL, 0};
    rhs = (struct autil_vstr){NULL, 0};
    EXPECT(autil_vstr_cmp(&lhs, &rhs) == 0);
    EXPECT(autil_vstr_vpcmp(&lhs, &rhs) == 0);

    lhs = AUTIL_VSTR_LITERAL("A");
    rhs = AUTIL_VSTR_LITERAL("A");
    EXPECT(autil_vstr_cmp(&lhs, &rhs) == 0);
    EXPECT(autil_vstr_vpcmp(&lhs, &rhs) == 0);

    lhs = AUTIL_VSTR_LITERAL("A");
    rhs = AUTIL_VSTR_LITERAL("AA");
    EXPECT(autil_vstr_cmp(&lhs, &rhs) < 0);
    EXPECT(autil_vstr_vpcmp(&lhs, &rhs) < 0);

    lhs = AUTIL_VSTR_LITERAL("AA");
    rhs = AUTIL_VSTR_LITERAL("A");
    EXPECT(autil_vstr_cmp(&lhs, &rhs) > 0);
    EXPECT(autil_vstr_vpcmp(&lhs, &rhs) > 0);

    lhs = AUTIL_VSTR_LITERAL("A");
    rhs = AUTIL_VSTR_LITERAL("B");
    EXPECT(autil_vstr_cmp(&lhs, &rhs) < 0);
    EXPECT(autil_vstr_vpcmp(&lhs, &rhs) < 0);

    lhs = AUTIL_VSTR_LITERAL("B");
    rhs = AUTIL_VSTR_LITERAL("A");
    EXPECT(autil_vstr_cmp(&lhs, &rhs) > 0);
    EXPECT(autil_vstr_vpcmp(&lhs, &rhs) > 0);

    lhs = AUTIL_VSTR_LITERAL("FooBar");
    rhs = (struct autil_vstr){"Fo\0Bar", 6};
    EXPECT(autil_vstr_cmp(&lhs, &rhs) > 0);
    EXPECT(autil_vstr_vpcmp(&lhs, &rhs) > 0);

    lhs = AUTIL_VSTR_LITERAL("Fo\0\0ar");
    rhs = (struct autil_vstr){"Fo\0Bar", 6};
    EXPECT(autil_vstr_cmp(&lhs, &rhs) < 0);
    EXPECT(autil_vstr_vpcmp(&lhs, &rhs) < 0);

    return EXIT_SUCCESS;
}
