// Run with:
//      LC_ALL=C tcc -run generate-ctype-tests.c
// from the test/ directory.
#define AUTIL_IMPLEMENTATION
#include "../autil.h"

#include <ctype.h>

// Number of characters in an 8-bit extended ascii character.
#define EASCII_RANGE 256

typedef int (*ctypefunc)(int c);

static void
make_istest(char const* func_name)
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

    size_t const buf_size = AUTIL_FMT_COUNT(fmt, func_name) + 1;
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
make_isexpected(char const* func_name, ctypefunc f)
{
    unsigned char buf[EASCII_RANGE + 1] = {0};
    for (int i = 0; i < EASCII_RANGE; ++i) {
        buf[i] = f(i) ? '1' : '0';
    }
    buf[AUTIL_ARRAY_COUNT(buf) - 1] = '\n';

    size_t const path_count = strlen(func_name) + strlen(".expected") + 1;
    char* const path = xalloc(NULL, path_count);
    sprintf(path, "%s.expected", func_name);
    file_write(path, buf, AUTIL_ARRAY_COUNT(buf));

    xalloc(path, XALLOC_FREE);
}

static void
make_isfiles(char const* func_name, ctypefunc f)
{
    make_istest(func_name);
    make_isexpected(func_name, f);
}

static void
make_totest(char const* func_name)
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
"        printf(\"%%c\", %s(i));\n"
"    }\n"
"    fputc('\\n', stdout);\n"
"    return EXIT_SUCCESS;\n"
"}\n";
    // clang-format on

    size_t const buf_size = AUTIL_FMT_COUNT(fmt, func_name) + 1;
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
make_toexpected(char const* func_name, ctypefunc f)
{
    unsigned char buf[EASCII_RANGE + 1] = {0};
    for (int i = 0; i < EASCII_RANGE; ++i) {
        buf[i] = f(i);
    }
    buf[AUTIL_ARRAY_COUNT(buf) - 1] = '\n';

    size_t const path_count = strlen(func_name) + strlen(".expected") + 1;
    char* const path = xalloc(NULL, path_count);
    sprintf(path, "%s.expected", func_name);
    file_write(path, buf, AUTIL_ARRAY_COUNT(buf));

    xalloc(path, XALLOC_FREE);
}

static void
make_tofiles(char const* func_name, ctypefunc f)
{
    make_totest(func_name);
    make_toexpected(func_name, f);
}

int
main(void)
{
    make_isfiles("autil_isalnum", isalnum);
    make_isfiles("autil_isalpha", isalpha);
    make_isfiles("autil_isblank", isblank);
    make_isfiles("autil_iscntrl", iscntrl);
    make_isfiles("autil_isdigit", isdigit);
    make_isfiles("autil_isgraph", isgraph);
    make_isfiles("autil_islower", islower);
    make_isfiles("autil_isprint", isprint);
    make_isfiles("autil_ispunct", ispunct);
    make_isfiles("autil_isspace", isspace);
    make_isfiles("autil_isupper", isupper);
    make_isfiles("autil_isxdigit", isxdigit);

    make_tofiles("autil_tolower", tolower);
    make_tofiles("autil_toupper", toupper);

    return EXIT_SUCCESS;
}
