#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "perf-bigint-divrem.h"

#define LHS_CSTR \
    "0x" \
    "e2c3c8d9712c67c7024092726e8b4fa09c6d002f119f62d641dd19966dbc5b8d" \
    "65880803fac12b1795463549b3eb0711affce7e8210ac36553d0bbff99305413"
#define RHS_CSTR \
    "0x" \
    "77c4968b242a11e6aacdc1c0b3c28d914832df8e605ddd95bd7d1ceec847ef8f"

int
main(void)
{
    measure_bigint_divrem(__FILE__, LHS_CSTR, RHS_CSTR);
    return EXIT_SUCCESS;
}
