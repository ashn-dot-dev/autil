#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bstr lhs = {0};
    struct autil_bstr rhs = {0};

    int const local_ptr_cmp = autil_bstr_cmp(
        AUTIL_BSTR_LOCAL_PTR("foo", 3), AUTIL_BSTR_LOCAL_PTR("bar", 3));
    EXPECT(local_ptr_cmp != 0);

    lhs = (struct autil_bstr){NULL, 0};
    rhs = (struct autil_bstr){NULL, 0};
    EXPECT(autil_bstr_cmp(&lhs, &rhs) == 0);
    EXPECT(autil_bstr_vpcmp(&lhs, &rhs) == 0);

    lhs = AUTIL_BSTR_LITERAL("A");
    rhs = AUTIL_BSTR_LITERAL("A");
    EXPECT(autil_bstr_cmp(&lhs, &rhs) == 0);
    EXPECT(autil_bstr_vpcmp(&lhs, &rhs) == 0);

    lhs = AUTIL_BSTR_LITERAL("A");
    rhs = AUTIL_BSTR_LITERAL("AA");
    EXPECT(autil_bstr_cmp(&lhs, &rhs) < 0);
    EXPECT(autil_bstr_vpcmp(&lhs, &rhs) < 0);

    lhs = AUTIL_BSTR_LITERAL("AA");
    rhs = AUTIL_BSTR_LITERAL("A");
    EXPECT(autil_bstr_cmp(&lhs, &rhs) > 0);
    EXPECT(autil_bstr_vpcmp(&lhs, &rhs) > 0);

    lhs = AUTIL_BSTR_LITERAL("A");
    rhs = AUTIL_BSTR_LITERAL("B");
    EXPECT(autil_bstr_cmp(&lhs, &rhs) < 0);
    EXPECT(autil_bstr_vpcmp(&lhs, &rhs) < 0);

    lhs = AUTIL_BSTR_LITERAL("B");
    rhs = AUTIL_BSTR_LITERAL("A");
    EXPECT(autil_bstr_cmp(&lhs, &rhs) > 0);
    EXPECT(autil_bstr_vpcmp(&lhs, &rhs) > 0);

    lhs = AUTIL_BSTR_LITERAL("FooBar");
    rhs = (struct autil_bstr){"Fo\0Bar", 6};
    EXPECT(autil_bstr_cmp(&lhs, &rhs) > 0);
    EXPECT(autil_bstr_vpcmp(&lhs, &rhs) > 0);

    lhs = AUTIL_BSTR_LITERAL("Fo\0\0ar");
    rhs = (struct autil_bstr){"Fo\0Bar", 6};
    EXPECT(autil_bstr_cmp(&lhs, &rhs) < 0);
    EXPECT(autil_bstr_vpcmp(&lhs, &rhs) < 0);

    return EXIT_SUCCESS;
}
