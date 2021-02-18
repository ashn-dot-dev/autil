#define AUTIL_IMPLEMENTATION
#include "../autil.h"

static void
usage(void);
static struct autil_bigint*
argparse(int argc, char** argv);

int
main(int argc, char** argv)
{
    struct autil_bigint* N = argparse(argc, argv);
    assert(autil_bigint_cmp(N, AUTIL_BIGINT_ZERO) >= 0);

    struct autil_bigint* const prev0 = autil_bigint_new(AUTIL_BIGINT_ZERO);
    struct autil_bigint* const prev1 = autil_bigint_new(AUTIL_BIGINT_POS_ONE);
    struct autil_bigint* const next = autil_bigint_new(prev0);
    while (autil_bigint_cmp(N, AUTIL_BIGINT_ZERO) > 0) {
        autil_bigint_add(next, prev1, prev0);
        autil_bigint_assign(prev0, prev1);
        autil_bigint_assign(prev1, next);
        autil_bigint_sub(N, N, AUTIL_BIGINT_POS_ONE);
    }

    char* const cstr = autil_bigint_to_new_cstr(prev0, NULL);
    puts(cstr);

    autil_bigint_del(N);
    autil_bigint_del(prev0);
    autil_bigint_del(prev1);
    autil_bigint_del(next);
    autil_xalloc(cstr, AUTIL_XALLOC_FREE);
    return EXIT_SUCCESS;
}

static void
usage(void)
{
    // clang-format off
    fputs(
        "Usage: bigint-fib [OPTION]... N"                        "\n"
        "Output the Nth Fibonacci number to stdout."             "\n"
        "Options:"                                               "\n"
        "  -h, --help       Display usage information and exit." "\n"
        , stderr
    );
    // clang-format on
}

static struct autil_bigint*
argparse(int argc, char** argv)
{
    struct autil_bigint* N = NULL;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage();
            exit(EXIT_SUCCESS);
        }
        if (strncmp(argv[i], "-", 1) == 0 || strncmp(argv[i], "--", 2) == 0) {
            autil_fatalf("Unrecognized command line option '%s'", argv[i]);
        }

        if (N != NULL) {
            autil_bigint_del(N);
            autil_fatalf("Multiple integers specified", argv[i]);
        }
        if ((N = autil_bigint_new_cstr(argv[i])) == NULL) {
            autil_fatalf("Failed to parse integer '%s'", argv[i]);
        }
    }
    if (N == NULL) {
        usage();
        exit(EXIT_FAILURE);
    }

    return N;
}
