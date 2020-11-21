#define AUTIL_IMPLEMENTATION
#include "../autil.h"

static struct autil_vec* stack = NULL;

static void
init(void);
static void
fini(void);

static struct autil_bigint*
parse_int(char const* cstr);
static void
push(struct autil_bigint* num);
static struct autil_bigint*
pop(void);

int
main(int argc, char** argv)
{
    init();
    struct autil_bigint* x = NULL;
    struct autil_bigint* y = NULL;

    for (int argi = 1; argi < argc; ++argi) {
        char const* const arg = argv[argi];
        if (strcmp(arg, "+") == 0) {
            y = pop();
            x = pop();
            autil_bigint_add(x, x, y);
            autil_bigint_del(y);
            push(x);
            continue;
        }
        if (strcmp(arg, "-") == 0) {
            y = pop();
            x = pop();
            autil_bigint_sub(x, x, y);
            autil_bigint_del(y);
            push(x);
            continue;
        }
        if (strcmp(arg, "*") == 0 || strcmp(arg, "x") == 0) {
            y = pop();
            x = pop();
            autil_bigint_mul(x, x, y);
            autil_bigint_del(y);
            push(x);
            continue;
        }
        if (strcmp(arg, "/") == 0) {
            y = pop();
            x = pop();
            autil_bigint_divrem(x, NULL, x, y);
            autil_bigint_del(y);
            push(x);
            continue;
        }
        if (strcmp(arg, "%") == 0) {
            y = pop();
            x = pop();
            autil_bigint_divrem(NULL, x, x, y);
            autil_bigint_del(y);
            push(x);
            continue;
        }
        push(parse_int(arg));
    }

    if (autil_vec_count(stack) != 1) {
        autil_errorf("Final stack has more than one element");
        return EXIT_FAILURE;
    }

    struct autil_bigint** presult =
        autil_vec_ref(stack, autil_vec_count(stack) - 1);
    char* const s = autil_bigint_to_cstr(*presult, NULL);
    puts(s);
    autil_xalloc(s, AUTIL_XALLOC_FREE);
    return EXIT_SUCCESS;
}

static void
init(void)
{
    stack = autil_vec_new(sizeof(struct autil_bigint*));
    atexit(fini);
}

static void
fini(void)
{
    for (size_t i = 0; i < autil_vec_count(stack); ++i) {
        struct autil_bigint* const num =
            AUTIL_DEREF_PTR(struct autil_bigint*, autil_vec_ref(stack, i));
        autil_bigint_del(num);
    }
    autil_vec_del(stack);
}

static struct autil_bigint*
parse_int(char const* cstr)
{
    struct autil_bigint* const num = autil_bigint_new_cstr(cstr);
    if (num == NULL) {
        autil_fatalf("Failed to parse integer '%s'", cstr);
    }
    return num;
}

static void
push(struct autil_bigint* num)
{
    autil_vec_insert(stack, autil_vec_count(stack), &num);
}

static struct autil_bigint*
pop(void)
{
    if (autil_vec_count(stack) == 0) {
        autil_fatalf("Attempted to pop empty stack");
    }

    struct autil_bigint* num = NULL;
    autil_vec_remove(stack, autil_vec_count(stack) - 1, &num);
    return num;
}
