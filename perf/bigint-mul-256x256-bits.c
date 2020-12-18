#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "perf-bigint-mul.h"

#define LHS_CSTR                                                               \
    "0x"                                                                       \
    "b575ff5679c2a82bf42b07dea3610306e6a2c4ce53d72bc80270ac703c21980b"
#define RHS_CSTR                                                               \
    "0x"                                                                       \
    "9193af55559e9ac0195b9acd4268e93b4c46ec16f42f16429a6f0a936aa1e00c"

int
main(void)
{
    measure_bigint_mul(__FILE__, LHS_CSTR, RHS_CSTR);
    return EXIT_SUCCESS;
}
