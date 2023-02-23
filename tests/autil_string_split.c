#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

static void
cleanup(struct autil_string* string, struct autil_string** split)
{
    autil_string_del(string);
    for (size_t i = 0; i < autil_sbuf_count(split); ++i) {
        autil_string_del(split[i]);
    }
    autil_sbuf_fini(split);
}

static void
dump(
    struct autil_string const* string,
    char const* delimiter,
    struct autil_string* const* split)
{
    assert(autil_sbuf_count(split) >= 1);

    printf("\"%s\" split on \"%s\" =>", autil_string_start(string), delimiter);
    for (size_t i = 0; i < autil_sbuf_count(split); ++i) {
        printf(" \"%s\"", autil_string_start(split[i]));
    }

    fputc('\n', stdout);
}

static void
test(char const* input, char const* delimiter)
{
    struct autil_string* const string = autil_string_new_cstr(input);
    struct autil_sbuf(autil_string*) const split =
        autil_string_split(string, delimiter, strlen(delimiter));
    dump(string, delimiter, split);
    cleanup(string, split);
}

int
main(void)
{
    test("", "");
    test("A", "");
    test("A", "A");
    test("A", "B");
    test("AA", "A");
    test("A A", " ");
    test("ABCBB", "B");
    test("ABC", "D");
    test("ABC", "FOOBAR");
}
