#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "perf.h"

#define LHS_CSTR \
    "0x" \
    "497fe1a1554a9ae2e1baa6094b5992c53384e499bf275fbfd124229d0b0267e0" \
    "e1080f5214653999e577b77615e80b0fce1dd56eaa10fa44389c39fa8b7ae862" \
    "86b4ea44819fcc019091ce172d7a7fdb0e562af664404aed877bbf705134dfa4" \
    "265f45c57ef4738394b52e66bd922e184771687d5db342be8da3bdf92850474a" \
    "b4a0eeb962080c7f520d4ce469aaed464368e749eec4d2e9ac7ae004cd15e213" \
    "6eca0eb2c3d2f5f2c8fd9831f2f178d409b628fa7ea616835efcf6498da2ea28" \
    "aedbbc3b3e13de174df6cf367260e2828ab02781623e4250593c502b95008379" \
    "47deebe83408098dffb36df41b296fcf8a95ee77f4155dad3a1a4d2047b32088" \
    "21a7df962960bb69e07d9d0bbc487b280f7f741f95d3e51dd3e7752f6de1a7ac" \
    "90a3c583b1720ceef1ea67bb8172833625dab9a87a4b7843691b7330d71ef1c4" \
    "9a706dae65aeb5f72d04fa6cf49bbbe3857ced8679a0fd7bc78fdf558e3c173b" \
    "4056cdb051a2de6b5776c0b4aee7f3030aef1fcc6958fb9e6b2ca466515808c9" \
    "5e14af811346c54221504b094e3703267f066e27470ce5da2932bb1739850ff0" \
    "1db13e86a137e99bdc8407d1e30770615a732ba105f519a29187e9b80f5667e9" \
    "9dd7d7954d9f150a834919011497dba207e93d81ca5650af40a9406476ca5b17" \
    "2d0e935dd75990a6c59dfa8327bd65d483a4c290a513efab45eed756c7cb9291"
#define RHS_CSTR \
    "0x" \
    "7ea6f594fa385eb3139650359e590998b2dac2bc1c5b1eefb06acb8782c4545e" \
    "5c758cc1d86da042cb558ec0048f5650ce36f9eef418402450fb79f0cad57996" \
    "49ae27236bbda198b6609a5b87d1d8caa8d8f88c922d0c8b8688453cda5e5da0" \
    "9d1ed82a2e49d3e446f5e8705b927d37abca4c07907f3ca6d65b88c65a494cdb" \
    "c47c73d42215e846fde95234e3da72c0255553226a69dc16dc2e0f1eabd055ca" \
    "8d2a68a2714dd426cd57abb360611b2b56982eb0564cba37e43d682a854b45df" \
    "79be6f0ec5c24e033a213d83e85de9618bab4684ff66ee27d44cfa8d07ab9755" \
    "7640994957684521dd59ce87ca1657626c42fe44cc855d73e56d9603f1ec8fa5" \
    "638313f66a12bfacd9e5e227c6a108b8fda4696399f960f190af81d6fcf1e1f5" \
    "ef6e8ca8c5bd619003c9a6726531abae0fbf9e6d837256f4d16989df12ef4540" \
    "7f2dc1391f5f080c40c1e6de68b37946adfd34ead07edbe83c5fa0f6c1a818a7" \
    "767b1e6b5d330cab00fae5e9c7533d7b23d3cc1ebedad7485caf377ee3cb6ed5" \
    "9181971b9c154b915988ed695c5886db632860cd2207afe1cf2e0b2b5482bfa0" \
    "aa839e927a54364f34caf095166e277a6fd6ac992621196be5677b0bb38c376d" \
    "24a56e9d24f7e1801061a760d3d3fd4d97247b9cdc8cbcb7af43b64221a26a79" \
    "376ec844f3b2f1fd9478d730d7d6b3c8a9ac7504edcbabaff59cde5c57fcca82"

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
