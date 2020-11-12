#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

int
main(void)
{
    struct autil_bigint* LHS = NULL;
    struct autil_bigint* RHS = NULL;
#define TEST_VALID(lhs_, rhs_, op_) \
    LHS = autil_bigint_new_cstr(lhs_); \
    RHS = autil_bigint_new_cstr(rhs_); \
    ASSERT(LHS != NULL); \
    ASSERT(RHS != NULL); \
    EXPECT(autil_bigint_cmp(LHS, RHS) op_ 0); \
    EXPECT(autil_bigint_vpcmp(LHS, RHS) op_ 0); \
    autil_bigint_del(LHS); \
    autil_bigint_del(RHS); \

    TEST_VALID("0", "0", ==);
    TEST_VALID("+1", "+1", ==);
    TEST_VALID("-1", "-1", ==);
    TEST_VALID("+0xbeefbeefbeefbeef", "+0xbeefbeefbeefbeef", ==);
    TEST_VALID("-0xbeefbeefbeefbeef", "-0xbeefbeefbeefbeef", ==);

    TEST_VALID("+1", "0", >);
    TEST_VALID("0", "+1", <);
    TEST_VALID("-1", "0", <);
    TEST_VALID("0", "-1", >);

    TEST_VALID("+0xbeefbeefbeefbeef", "+0xcafed00d5", >);
    TEST_VALID("+0xbeefbeefbeefbeef", "-0xcafed00d5", >);
    TEST_VALID("-0xbeefbeefbeefbeef", "-0xcafed00d5", <);
    TEST_VALID("-0xbeefbeefbeefbeef", "+0xcafed00d5", <);
}
