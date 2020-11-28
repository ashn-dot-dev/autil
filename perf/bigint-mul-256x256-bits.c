#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "perf.h"

#define LHS_CSTR \
    "0x" \
    "b575ff5679c2a82bf42b07dea3610306e6a2c4ce53d72bc80270ac703c21980b"
#define RHS_CSTR \
    "0x" \
    "9193af55559e9ac0195b9acd4268e93b4c46ec16f42f16429a6f0a936aa1e00c"

int
main(void)
{
    struct autil_bigint* const LHS = autil_bigint_new_cstr(LHS_CSTR);
    struct autil_bigint* const RHS = autil_bigint_new_cstr(RHS_CSTR);
    struct autil_bigint* const RES = autil_bigint_new(AUTIL_BIGINT_ZERO);

    clock_t const start = clock();
    autil_bigint_mul(RES, LHS, RHS);
    clock_t const end = clock();
    printf("%s: %0.6f sec\n", __FILE__, elapsed_sec(start, end));

    autil_bigint_del(LHS);
    autil_bigint_del(RHS);
    autil_bigint_del(RES);
    return EXIT_SUCCESS;
}
