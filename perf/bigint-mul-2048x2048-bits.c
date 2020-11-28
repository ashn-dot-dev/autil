#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "perf-bigint-mul.h"

#define LHS_CSTR \
    "0x" \
    "29543bf0bd7b0428bbba161ee0361f62634527551f5c693797e6def74056854b" \
    "4de05cda266fc30b6bb5536219b8ecd49973fe0ded3beba53872ef0fdefca39f" \
    "cfea3db4e9bad1429fe50d63b4a0b79635e9b0cb36114c54b98aa0c219241adf" \
    "cf63b44109ff3d4d0a92caacb367a1e7db9d12cd6e6a847337982555e111b815" \
    "8f5431a024bcd6864b05d0520fd5f294e5894674f24a583ef595002f8b468be4" \
    "6f9abd7453af50b1d5094a7b8da9d067c463af2f0a2194d717322d4d5eb856b9" \
    "4de916db3a1b140ae013724b8f9cfda4b80ec5652adeed35ad20d927cfbafbe1" \
    "9fac9562a4578d8a12279ad6d67417a01f10a46234c4c3352acb56f678bfd2be"
#define RHS_CSTR \
    "0x" \
    "6dc35cb0423f5d4340ba048646adcfa5c6f15b3776c425b98ca7a71132582ef6" \
    "e4fb71f7303b74178610ee1fbe3c45dfd661dca9cca62d72aca5c0971afec632" \
    "f1c2daf0fe1cee0afae9417e5de89483dfb05b2348e05e42a4b8a5d017c61dce" \
    "5b958105887219a1ab0382376d4a80d276826de4d8c06c56ef03408a2f6e8261" \
    "191dba7ff0ba167636012bea9ced1a963695e697c3801b8ed2bcc77304cad24b" \
    "b715d2c25ae9da66bec183aeb8ae0a660e7fe448404d2e968e6560ae4b22aec4" \
    "bfef8068e2acd9744b75bde4f81fae0e720cc9acdc672a02077175b751bfd806" \
    "7cfe970087036f85d92fdfba476e5b75ae182f76d9eca50ef3ec9342d78a1d23"

int
main(void)
{
    measure_bigint_mul(__FILE__, LHS_CSTR, RHS_CSTR);
    return EXIT_SUCCESS;
}
