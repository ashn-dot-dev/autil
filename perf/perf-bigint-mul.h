#ifndef PERF_BIGINT_MUL_H_INCLUDED
#define PERF_BIGINT_MUL_H_INCLUDED

#include "perf.h"

static void
measure_bigint_mul(char const* file, char const* lhs_cstr, char const* rhs_cstr)
{
    struct autil_bigint* const LHS = autil_bigint_new_cstr(lhs_cstr);
    struct autil_bigint* const RHS = autil_bigint_new_cstr(rhs_cstr);
    struct autil_bigint* const RES = autil_bigint_new(AUTIL_BIGINT_ZERO);

    clock_t const start = clock();
    autil_bigint_mul(RES, LHS, RHS);
    clock_t const end = clock();
    double const elapsed = elapsed_sec(start, end);

    printf("%s: %0.6f sec\n", file, elapsed);

    autil_bigint_del(LHS);
    autil_bigint_del(RHS);
    autil_bigint_del(RES);
}

#endif // PERF_BIGINT_MUL_H_INCLUDED
