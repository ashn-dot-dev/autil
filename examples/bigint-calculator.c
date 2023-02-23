#define AUTIL_IMPLEMENTATION
#include "../autil.h"

static void
stack_push(struct autil_vec* stack, struct autil_bigint* num);
static struct autil_bigint*
stack_pop(struct autil_vec* stack);
static void
stack_print(struct autil_vec const* stack);

static void
eval_line(char const* line, size_t line_size);
static int
eval_token(struct autil_vec* stack, char const* token);

int
main(void)
{
    void* line;
    size_t line_size;
    while (autil_stream_read_line(stdin, &line, &line_size) == 0) {
        eval_line(line, line_size);
        autil_xalloc(line, AUTIL_XALLOC_FREE);
    }

    return EXIT_SUCCESS;
}

static void
stack_push(struct autil_vec* stack, struct autil_bigint* num)
{
    autil_vec_insert(stack, autil_vec_count(stack), &num);
}

static struct autil_bigint*
stack_pop(struct autil_vec* stack)
{
    if (autil_vec_count(stack) == 0) {
        autil_errorf("Attempted to pop empty stack");
        return NULL;
    }

    struct autil_bigint* num = NULL;
    autil_vec_remove(stack, autil_vec_count(stack) - 1, &num);
    return num;
}

static void
stack_print(struct autil_vec const* stack)
{
    if (autil_vec_count(stack) == 0) {
        puts("<empty stack>");
        return;
    }

    for (size_t i = 0; i < autil_vec_count(stack); ++i) {
        struct autil_bigint const* num = AUTIL_DEREF_PTR(
            struct autil_bigint const*, autil_vec_ref_const(stack, i));
        char* const s = autil_bigint_to_new_cstr(num, NULL);
        printf("%s%s", i != 0 ? " " : "", s);
        autil_xalloc(s, AUTIL_XALLOC_FREE);
    }
    fputc('\n', stdout);
}

void
eval_line(char const* line, size_t line_size)
{
    struct autil_string* const string = autil_string_new(line, line_size);
    struct autil_vec* const stack = autil_vec_new(sizeof(struct autil_bigint*));

    autil_string_trim(string);
    autil_sbuf(struct autil_string*) const tokens =
        autil_string_split(string, " ", 1);

    int err = 0;
    for (size_t i = 0; i < autil_sbuf_count(tokens); ++i) {
        if (autil_string_count(tokens[i]) == 0) {
            continue;
        }
        if ((err = eval_token(stack, autil_string_start(tokens[i])))) {
            break;
        }
    }
    if (!err) {
        stack_print(stack);
    }

    autil_string_del(string);
    for (size_t i = 0; i < autil_sbuf_count(tokens); ++i) {
        autil_string_del(tokens[i]);
    }
    autil_sbuf_fini(tokens);
    for (size_t i = 0; i < autil_vec_count(stack); ++i) {
        autil_bigint_del(
            AUTIL_DEREF_PTR(struct autil_bigint*, autil_vec_ref(stack, i)));
    }
    autil_vec_del(stack);
}

static int
eval_token(struct autil_vec* stack, char const* token)
{
    struct autil_bigint* const res = autil_bigint_new(AUTIL_BIGINT_ZERO);
    struct autil_bigint* lhs = NULL;
    struct autil_bigint* rhs = NULL;

    if (strcmp(token, "+") == 0) {
        if ((rhs = stack_pop(stack)) == NULL) {
            goto error;
        }
        if ((lhs = stack_pop(stack)) == NULL) {
            goto error;
        }
        autil_bigint_add(res, lhs, rhs);
    }
    else if (strcmp(token, "-") == 0) {
        if ((rhs = stack_pop(stack)) == NULL) {
            goto error;
        }
        if ((lhs = stack_pop(stack)) == NULL) {
            goto error;
        }
        autil_bigint_sub(res, lhs, rhs);
    }
    else if (strcmp(token, "*") == 0) {
        if ((rhs = stack_pop(stack)) == NULL) {
            goto error;
        }
        if ((lhs = stack_pop(stack)) == NULL) {
            goto error;
        }
        autil_bigint_mul(res, lhs, rhs);
    }
    else if (strcmp(token, "/") == 0) {
        if ((rhs = stack_pop(stack)) == NULL) {
            goto error;
        }
        if ((lhs = stack_pop(stack)) == NULL) {
            goto error;
        }
        if (autil_bigint_cmp(rhs, AUTIL_BIGINT_ZERO) == 0) {
            autil_errorf("Division with denominator zero");
            goto error;
        }
        autil_bigint_divrem(res, NULL, lhs, rhs);
    }
    else if (strcmp(token, "%") == 0) {
        if ((rhs = stack_pop(stack)) == NULL) {
            goto error;
        }
        if ((lhs = stack_pop(stack)) == NULL) {
            goto error;
        }
        if (autil_bigint_cmp(rhs, AUTIL_BIGINT_ZERO) == 0) {
            autil_errorf("Remainder with denominator zero");
            goto error;
        }
        autil_bigint_divrem(NULL, res, lhs, rhs);
    }
    else {
        struct autil_bigint* const parsed = autil_bigint_new_cstr(token);
        if (parsed == NULL) {
            autil_errorf("Failed to parse '%s'", token);
            goto error;
        }
        autil_bigint_assign(res, parsed);
        autil_bigint_del(parsed);
    }

    stack_push(stack, res);
    autil_bigint_del(lhs);
    autil_bigint_del(rhs);
    return 0;

error:
    autil_bigint_del(res);
    autil_bigint_del(lhs);
    autil_bigint_del(rhs);
    return -1;
}
