#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "perf-bigint-divrem.h"

#define LHS_CSTR                                                               \
    "0x"                                                                       \
    "4f82cd1d7a46639fb8d8860b116876cf7c6c6e6048da0ae90bd6646a389d8b46"         \
    "2eae4b33b21f9dfdc79da9fc7b0dfe90cbf9951d8a43657943e50d5e698638c9"         \
    "7d476014d231c460fa3d46d8e18b2d9fb7b1f177f6c929d4f4713d752b03a776"         \
    "0863cf5e1cb70a30baf2629bd71a17c335f6cc21a70dda21a073a752841b31cb"         \
    "4cde0070dc3048dc0850560c5a8fe41f4e6d81274d20caadb28026096f983aa1"         \
    "429fcc84894e2071d06b68df2735851381784e46935e3a1b23b625b117875294"         \
    "e13b06606ee27a5b56067d1fa36565ac6e5e6be842fed3b20b2357cf7b23b2bc"         \
    "af93cd125a98ec63710c2f4a993add24b20a73878e4397cd3352e73c54adb54b"
#define RHS_CSTR                                                               \
    "0x"                                                                       \
    "915d7700ec4b9db9341b64cee62daf6acfefc9a50d2b7014138d31e66e0fa519"         \
    "af648d0b216f3668736ecb26ada53df5a82db929a638ab704d90b5f76481ec56"         \
    "e4bbdcebc328226350b1f8a801c26eae7d7f4ac8be4834e3f6a6bdf752ca5292"         \
    "31f0274cf372c90bec050aaafc7503f0265635c3d5fd0715194ccd9f27b0f75b"

int
main(void)
{
    measure_bigint_divrem(__FILE__, LHS_CSTR, RHS_CSTR);
    return EXIT_SUCCESS;
}
