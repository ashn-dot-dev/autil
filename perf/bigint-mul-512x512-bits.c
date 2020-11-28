#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "perf-bigint-mul.h"

#define LHS_CSTR \
    "0x" \
    "e89f021c4c87495d599fc8b273cf4f5c999aebdf2469b0e726f142d033eb4076" \
    "94c770e2ccda41c0dd4e148c4f0b3f67f02ba94e479827d1a33a263c5262145f"
#define RHS_CSTR \
    "0x" \
    "212a3b90095d5cf0478733c9b8d46aecaa10e6c459dfce35554bf05d780167e2" \
    "ae850700dc0e63ea600a8aae7c59904260bd00f66b5954c9636dea33571cb7bf"

int
main(void)
{
    measure_bigint_mul(__FILE__, LHS_CSTR, RHS_CSTR);
    return EXIT_SUCCESS;
}
