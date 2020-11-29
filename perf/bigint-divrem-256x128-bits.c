#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "perf-bigint-divrem.h"

#define LHS_CSTR \
    "0x" \
    "4c43b61301514185ff18469f97a18b19454d1f71f63b863fc972ec2cdf08bf02"
#define RHS_CSTR \
    "0x" \
    "320edd3fe528bda73a89ed99adb14f2f"

int
main(void)
{
    measure_bigint_divrem(__FILE__, LHS_CSTR, RHS_CSTR);
    return EXIT_SUCCESS;
}
