#define AUTIL_IMPLEMENTATION
#include "../autil.h"

struct autil_bigint* RES = NULL;
struct autil_bigint* LHS = NULL;
struct autil_bigint* RHS = NULL;

static void
test_autil_bigint_add__0_add_x(void)
{
    puts(__func__);
    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(RES, AUTIL_BIGINT_ZERO, AUTIL_BIGINT_POS_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(RES, AUTIL_BIGINT_ZERO, AUTIL_BIGINT_NEG_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    RHS = autil_bigint_new_cstr("0x1234123412341234");
    autil_bigint_add(RES, AUTIL_BIGINT_ZERO, RHS);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(RHS);
}

static void
test_autil_bigint_add__x_add_0(void)
{
    puts(__func__);

    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(RES, AUTIL_BIGINT_POS_ONE, AUTIL_BIGINT_ZERO);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(RES, AUTIL_BIGINT_NEG_ONE, AUTIL_BIGINT_ZERO);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    LHS = autil_bigint_new_cstr("0x1234123412341234");
    autil_bigint_add(RES, LHS, AUTIL_BIGINT_ZERO);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(LHS);
}

static void
test_autil_bigint_add__pos_add_pos(void)
{
    puts(__func__);

    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(RES, AUTIL_BIGINT_POS_ONE, AUTIL_BIGINT_POS_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    LHS = autil_bigint_new_cstr("0x3210321032103210");
    RHS = autil_bigint_new_cstr("0x4567456745674567");
    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(RES, LHS, RHS);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(LHS);
    autil_bigint_del(RHS);
}

static void
test_autil_bigint_add__neg_add_neg(void)
{
    puts(__func__);

    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(RES, AUTIL_BIGINT_NEG_ONE, AUTIL_BIGINT_NEG_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    LHS = autil_bigint_new_cstr("-0x3210321032103210");
    RHS = autil_bigint_new_cstr("-0x4567456745674567");
    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(RES, LHS, RHS);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(LHS);
    autil_bigint_del(RHS);
}

static void
test_autil_bigint_add__pos_add_neg(void)
{
    puts(__func__);

    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(RES, AUTIL_BIGINT_POS_ONE, AUTIL_BIGINT_NEG_ONE);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);

    LHS = autil_bigint_new_cstr("+0x4567456745674567");
    RHS = autil_bigint_new_cstr("-0x3210321032103210");
    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(RES, LHS, RHS);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(LHS);
    autil_bigint_del(RHS);
}

static void
test_autil_bigint_add__neg_add_pos(void)
{
    puts(__func__);
    struct autil_bigint* INT = NULL;

    INT = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(INT, AUTIL_BIGINT_NEG_ONE, AUTIL_BIGINT_POS_ONE);
    autil_bigint_dump(INT);
    autil_bigint_del(INT);

    LHS = autil_bigint_new_cstr("-0x4567456745674567");
    RHS = autil_bigint_new_cstr("+0x3210321032103210");
    RES = autil_bigint_new(AUTIL_BIGINT_ZERO);
    autil_bigint_add(RES, LHS, RHS);
    autil_bigint_dump(RES);
    autil_bigint_del(RES);
    autil_bigint_del(LHS);
    autil_bigint_del(RHS);
}

int
main(void)
{
    test_autil_bigint_add__0_add_x();
    test_autil_bigint_add__x_add_0();

    test_autil_bigint_add__pos_add_pos();
    test_autil_bigint_add__neg_add_neg();
    test_autil_bigint_add__pos_add_neg();
    test_autil_bigint_add__neg_add_pos();

    return EXIT_SUCCESS;
}
