#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "perf-bigint-mul.h"

#define LHS_CSTR \
    "0x" \
    "ee713a0f8c7195a32447e6d737305d93a72c8c0eb8cbb95b82cec54238bec521" \
    "5659a4f534f70f6abe0d541e0378e07d90b6882d4435bf1116051dd6f6582ddf" \
    "10449c15314138540b92bd49485b7504f637720378be0513a9cf695badf90ae9" \
    "5cf8053b13f6444f43d9349cc669e855c6796378204eda32927c8348a6f03f88"
#define RHS_CSTR \
    "0x" \
    "8c21c268a4d6737db48c03f995904e4039382c85b995141446f2e88b3b523c49" \
    "4d81849c1a0586874091773ed545bff8be7f23b935e09ba42cd5f1948d160f5d" \
    "19b87c099c0dcf7d83ce7cf0516d8e3f650ef09d1e81ae153286e82c4a883d74" \
    "d38e1901c0e7b392a8f32407cc96c0c97eec77181fc3ef459e3ccbf9e269fe74"

int
main(void)
{
    measure_bigint_mul(__FILE__, LHS_CSTR, RHS_CSTR);
    return EXIT_SUCCESS;
}
