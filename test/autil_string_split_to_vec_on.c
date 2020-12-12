#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

static void
cleanup(struct autil_string* string, struct autil_vec* split)
{
    autil_string_del(string);
    for (size_t i = 0; i < autil_vec_count(split); ++i) {
        autil_string_del(
            AUTIL_DEREF_PTR(struct autil_string*, autil_vec_ref(split, i)));
    }
}

static void
dump(struct autil_string const* string, struct autil_vec const* split)
{
    printf("\"%s\" =>", autil_string_start(string));
    for (size_t i = 0; i < autil_vec_count(split); ++i) {
        struct autil_string const* const* ps = autil_vec_ref_const(split, i);
        printf(" \"%s\"", autil_string_start(*ps));
    }
    fputc('\n', stdout);
}

int
main(void)
{
    struct autil_string* string = NULL;
    struct autil_vec* const split = autil_vec_new(sizeof(struct autil_string*));

    string = autil_string_new_cstr("");
    autil_string_split_to_vec_on_cstr(string, "", split);
    dump(string, split);
    cleanup(string, split);

    string = autil_string_new_cstr("A");
    autil_string_split_to_vec_on_cstr(string, "", split);
    dump(string, split);
    cleanup(string, split);

    string = autil_string_new_cstr("A");
    autil_string_split_to_vec_on_cstr(string, "A", split);
    dump(string, split);
    cleanup(string, split);

    string = autil_string_new_cstr("AA");
    autil_string_split_to_vec_on_cstr(string, "A", split);
    dump(string, split);
    cleanup(string, split);

    string = autil_string_new_cstr("A A");
    autil_string_split_to_vec_on_cstr(string, " ", split);
    dump(string, split);
    cleanup(string, split);

    string = autil_string_new_cstr("ABCBB");
    autil_string_split_to_vec_on_cstr(string, "B", split);
    dump(string, split);
    cleanup(string, split);

    string = autil_string_new_cstr("ABC");
    autil_string_split_to_vec_on_cstr(string, "D", split);
    dump(string, split);
    cleanup(string, split);

    string = autil_string_new_cstr("ABC");
    autil_string_split_to_vec_on_cstr(string, "FOOBAR", split);
    dump(string, split);
    cleanup(string, split);

    autil_vec_del(split);
}
