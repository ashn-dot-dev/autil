#define AUTIL_IMPLEMENTATION
#include "../autil.h"

struct autil_bigint* RES = NULL;
struct autil_bigint* LHS = NULL;
struct autil_bigint* RHS = NULL;

struct autil_bigint* POS_TWO = NULL;
struct autil_bigint* NEG_TWO = NULL;
struct autil_bigint* POS_0x1234 = NULL;
struct autil_bigint* NEG_0x1234 = NULL;

static void
test_autil_bigint_sub__0_sub_x(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, AUTIL_BIGINT_ZERO, AUTIL_BIGINT_POS_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, AUTIL_BIGINT_ZERO, AUTIL_BIGINT_NEG_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    RHS = autil_bigint_new_cstr("0x1234123412341234");
    autil_bigint_sub(RES, AUTIL_BIGINT_ZERO, RHS);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(RHS);
}

static void
test_autil_bigint_sub__x_sub_0(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, AUTIL_BIGINT_POS_ONE, AUTIL_BIGINT_ZERO);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, AUTIL_BIGINT_NEG_ONE, AUTIL_BIGINT_ZERO);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    LHS = autil_bigint_new_cstr("0x1234123412341234");
    autil_bigint_sub(RES, LHS, AUTIL_BIGINT_ZERO);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(LHS);
}

static void
test_autil_bigint_sub__pos_sub_pos(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, AUTIL_BIGINT_POS_ONE, AUTIL_BIGINT_POS_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, POS_TWO, AUTIL_BIGINT_POS_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, AUTIL_BIGINT_POS_ONE, POS_TWO);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, POS_0x1234, POS_TWO);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, POS_TWO, POS_0x1234);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
}

static void
test_autil_bigint_sub__neg_sub_neg(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, AUTIL_BIGINT_NEG_ONE, AUTIL_BIGINT_NEG_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, NEG_TWO, AUTIL_BIGINT_NEG_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, AUTIL_BIGINT_NEG_ONE, NEG_TWO);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, NEG_0x1234, NEG_TWO);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, NEG_TWO, NEG_0x1234);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
}

static void
test_autil_bigint_sub__pos_sub_neg(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, AUTIL_BIGINT_POS_ONE, AUTIL_BIGINT_NEG_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
}

static void
test_autil_bigint_sub__neg_sub_pos(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_sub(RES, AUTIL_BIGINT_NEG_ONE, AUTIL_BIGINT_POS_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
}

int
main(void)
{
    POS_TWO = autil_bigint_new_cstr("+2");
    NEG_TWO = autil_bigint_new_cstr("-2");
    POS_0x1234 = autil_bigint_new_cstr("+0x1234");
    NEG_0x1234 = autil_bigint_new_cstr("-0x1234");

    test_autil_bigint_sub__0_sub_x();
    test_autil_bigint_sub__x_sub_0();

    test_autil_bigint_sub__pos_sub_pos();
    test_autil_bigint_sub__neg_sub_neg();
    test_autil_bigint_sub__pos_sub_neg();
    test_autil_bigint_sub__neg_sub_pos();

    autil_bigint_del(POS_TWO);
    autil_bigint_del(NEG_TWO);
    autil_bigint_del(POS_0x1234);
    autil_bigint_del(NEG_0x1234);
    return EXIT_SUCCESS;
}
