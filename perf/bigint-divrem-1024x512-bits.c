#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "perf-bigint-divrem.h"

#define LHS_CSTR \
    "0x" \
    "2f7d30ad7a8123018e23d62301ebce5fd0c1fd853c0744ea4eed2287cd164e75" \
    "6f3fc5f741aabf18f5486251067138ac81efcb20002c66b575c6af5b5e6faea4" \
    "f24d3cd1f2d63cd2cb79ae0b2320eb40c2c74e3f070bc68404efae269bb8c61d" \
    "b20a785881712eb7fccf0fe274fc8df5527567bb54d4a8063cc64eadcad84810"
#define RHS_CSTR \
    "0x" \
    "239abca3b4c8f12dca295d239739d2fd86a86d71ae64739a49aa5e2a3d5ee032" \
    "a0168a36cf6e1c92c0b75e8188800337edadd240b647337f6e2cc5e49be6a135"

int
main(void)
{
    measure_bigint_divrem(__FILE__, LHS_CSTR, RHS_CSTR);
    return EXIT_SUCCESS;
}
