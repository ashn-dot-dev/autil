// Run with:
//      tcc -run generate-ctype-tests.c
// from the test/ directory.
#define AUTIL_IMPLEMENTATION
#include "../autil.h"

// Number of characters in an 8-bit extended ascii character.
#define EASCII_RANGE 256

typedef int (*isfunc)(int c);

static void
make_test(char const* func_name)
{
    // clang-format off
    char const* const fmt =
"#define AUTIL_IMPLEMENTATION\n"
"#include \"../autil.h\"\n"
"#include \"test.h\"\n"
"\n"
"int\n"
"main(void)\n"
"{\n"
"    for (int i = 0; i < 256; ++i) {\n"
"        printf(\"%%c\", %s(i) ? '1' : '0');\n"
"    }\n"
"    fputc('\\n', stdout);\n"
"    return EXIT_SUCCESS;\n"
"}\n";
    // clang-format on

    size_t const buf_size = FMT_COUNT(fmt, func_name) + 1;
    char* const buf = xalloc(NULL, buf_size);
    sprintf(buf, fmt, func_name);

    size_t const path_count = strlen(func_name) + strlen(".c") + 1;
    char* const path = xalloc(NULL, path_count);
    sprintf(path, "%s.c", func_name);
    file_write(path, buf, buf_size - 1);

    xalloc(buf, XALLOC_FREE);
    xalloc(path, XALLOC_FREE);
}

static void
make_expected(char const* func_name, isfunc f)
{
    unsigned char buf[EASCII_RANGE + 1] = {0};
    for (int i = 0; i < EASCII_RANGE; ++i) {
        buf[i] = f(i) ? '1' : '0';
    }
    buf[ARRAY_COUNT(buf) - 1] = '\n';

    size_t const path_count = strlen(func_name) + strlen(".expected") + 1;
    char* const path = xalloc(NULL, path_count);
    sprintf(path, "%s.expected", func_name);
    file_write(path, buf, ARRAY_COUNT(buf));

    xalloc(path, XALLOC_FREE);
}

static void
make_files(char const* func_name, isfunc f)
{
    make_test(func_name);
    make_expected(func_name, f);
}

int
main(void)
{
    make_files("is_alnum", isalnum);
    make_files("is_alpha", isalpha);
    make_files("is_blank", isblank);
    make_files("is_cntrl", iscntrl);
    make_files("is_digit", isdigit);
    make_files("is_graph", isgraph);
    make_files("is_lower", islower);
    make_files("is_print", isprint);
    make_files("is_punct", ispunct);
    make_files("is_space", isspace);
    make_files("is_upper", isupper);
    make_files("is_xdigit", isxdigit);

    return EXIT_SUCCESS;
}
