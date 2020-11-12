#define AUTIL_IMPLEMENTATION
#include "../autil.h"

struct autil_bigint* RES = NULL;
struct autil_bigint* LHS = NULL;
struct autil_bigint* RHS = NULL;

static void
test_autil_bigint_mul__0_mul_x(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_mul(RES, RES, AUTIL_BIGINT_POS_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_mul(RES, RES, AUTIL_BIGINT_NEG_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
}

static void
test_autil_bigint_mul__x_mul_0(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_mul(RES, AUTIL_BIGINT_POS_ONE, RES);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    autil_bigint_mul(RES, AUTIL_BIGINT_NEG_ONE, RES);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
}

static void
test_autil_bigint_mul__pos_mul_pos(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_mul(RES, AUTIL_BIGINT_POS_ONE, AUTIL_BIGINT_POS_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    LHS = autil_bigint_new_cstr("+0xffee");
    RHS = autil_bigint_new_cstr("+0xffee");
    autil_bigint_mul(RES, LHS, RHS);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(LHS);
    autil_bigint_del(RHS);
}

static void
test_autil_bigint_mul__neg_mul_neg(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_mul(RES, AUTIL_BIGINT_NEG_ONE, AUTIL_BIGINT_NEG_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    LHS = autil_bigint_new_cstr("-0xffee");
    RHS = autil_bigint_new_cstr("-0xffee");
    autil_bigint_mul(RES, LHS, RHS);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(LHS);
    autil_bigint_del(RHS);
}

static void
test_autil_bigint_mul__pos_mul_neg(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_mul(RES, AUTIL_BIGINT_POS_ONE, AUTIL_BIGINT_NEG_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    LHS = autil_bigint_new_cstr("+0xffee");
    RHS = autil_bigint_new_cstr("-0xffee");
    autil_bigint_mul(RES, LHS, RHS);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(LHS);
    autil_bigint_del(RHS);
}

static void
test_autil_bigint_mul__neg_mul_pos(void)
{
    puts(__func__);

    RES = autil_bigint_new();
    autil_bigint_mul(RES, AUTIL_BIGINT_NEG_ONE, AUTIL_BIGINT_POS_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new();
    LHS = autil_bigint_new_cstr("-0xffee");
    RHS = autil_bigint_new_cstr("+0xffee");
    autil_bigint_mul(RES, LHS, RHS);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(LHS);
    autil_bigint_del(RHS);
}

int
main(void)
{
    test_autil_bigint_mul__0_mul_x();
    test_autil_bigint_mul__x_mul_0();

    test_autil_bigint_mul__pos_mul_pos();
    test_autil_bigint_mul__neg_mul_neg();
    test_autil_bigint_mul__pos_mul_neg();
    test_autil_bigint_mul__neg_mul_pos();

    return EXIT_SUCCESS;
}
