#define AUTIL_IMPLEMENTATION
#include "../autil.h"

struct autil_bigint* DIV = NULL;
struct autil_bigint* REM = NULL;

struct autil_bigint* POS_0x44 = NULL;
struct autil_bigint* NEG_0x44 = NULL;
struct autil_bigint* POS_0xFFEE = NULL;
struct autil_bigint* NEG_0xFFEE = NULL;

static void
dump_divrem(struct autil_bigint const* DIV, struct autil_bigint* REM)
{
    printf("DIV => ");
    if (DIV != NULL) {
        autil_bigint_dump(DIV);
    }
    else {
        printf("NULL\n");
    }

    printf("REM => ");
    if (REM != NULL) {
        autil_bigint_dump(REM);
    }
    else {
        printf("NULL\n");
    }
}

static void
test_autil_bigint_div__pos_div_pos(void)
{
    puts(__func__);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, POS_0xFFEE, AUTIL_BIGINT_POS_ONE);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, POS_0xFFEE, POS_0x44);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, POS_0x44, POS_0xFFEE);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);
}

static void
test_autil_bigint_div__neg_div_neg(void)
{
    puts(__func__);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, NEG_0xFFEE, AUTIL_BIGINT_NEG_ONE);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, NEG_0xFFEE, NEG_0x44);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, NEG_0x44, NEG_0xFFEE);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);
}

static void
test_autil_bigint_div__pos_div_neg(void)
{
    puts(__func__);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, POS_0xFFEE, AUTIL_BIGINT_NEG_ONE);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, POS_0xFFEE, NEG_0x44);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, POS_0x44, NEG_0xFFEE);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);
}

static void
test_autil_bigint_div__neg_div_pos(void)
{
    puts(__func__);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, NEG_0xFFEE, AUTIL_BIGINT_POS_ONE);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, NEG_0xFFEE, POS_0x44);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);

    DIV = autil_bigint_new();
    REM = autil_bigint_new();
    autil_bigint_div(DIV, REM, NEG_0x44, POS_0xFFEE);
    dump_divrem(DIV, REM);
    autil_bigint_del(DIV);
    autil_bigint_del(REM);
}

int
main(void)
{
    POS_0x44 = autil_bigint_new_cstr("+0x44");
    NEG_0x44 = autil_bigint_new_cstr("-0x44");
    POS_0xFFEE = autil_bigint_new_cstr("+0xffee");
    NEG_0xFFEE = autil_bigint_new_cstr("-0xffee");

    test_autil_bigint_div__pos_div_pos();
    test_autil_bigint_div__neg_div_neg();
    test_autil_bigint_div__pos_div_neg();
    test_autil_bigint_div__neg_div_pos();

    autil_bigint_del(POS_0x44);
    autil_bigint_del(NEG_0x44);
    autil_bigint_del(POS_0xFFEE);
    autil_bigint_del(NEG_0xFFEE);
    return EXIT_SUCCESS;
}
