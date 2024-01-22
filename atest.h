/*
ATEST - ASHN'S TESTING LIBRARY - v2.0.0
    Single header file for unit testing in C99.
    This library has no dependencies outside of libc.


USAGE
    To create function and object definitions:
        #define ATEST_IMPLEMENTATION
    in exactly *ONE* translation unit before including this file:
        #define ATEST_IMPLEMENTATION
        #include "atest.h"

    To restrict function and object definitions to a single translation unit:
        #define ATEST_API static
    before including this file:
        #define ATEST_IMPLEMENTATION
        #define ATEST_API static
        #include "atest.h"


LICENSE
    Copyright (c) 2020 ashn <me@ashn.dev>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
    SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
    IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// HEADER SECTION ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef ATEST_H_INCLUDED
#define ATEST_H_INCLUDED

#ifndef ATEST_API
#    define ATEST_API extern
#endif

#include <stddef.h> /* NULL */

// Declare or define a test with the provided identifier.
//
// Examples:
//      ATEST_TEST(foo);                        // Forward declare test.
//      ATEST_TEST(foo){ /* test code here */ } // Define test.
#define ATEST_TEST(ident) void ident(void)

// Fail the currently running test and print the provided printf-style
// diagnostic message to stderr.
#define ATEST_FAIL(/* error message (fmt & args) */...)                        \
    atest__fail_(__FILE__, __LINE__, __VA_ARGS__)

// Fail the currently running test if the provided expression does not evaluate
// as truthy, then continue execution of the test.
#define ATEST_EXPECT(expr)                                                     \
    ATEST__IFNOT_(expr, ATEST_FAIL("expect failure: " #expr);)

// Fail the currently running test if the provided expression does not evaluate
// as truthy, in which case control will immediately return from the test.
#define ATEST_ASSERT(expr)                                                     \
    ATEST__IFNOT_(expr, ATEST_FAIL("assert failure: " #expr); return;)

// Run a test with the provided identifier.
#define ATEST_RUN(ident) atest__run_(#ident, ident, NULL, NULL)

// Run a test with the provided identifier, calling test-fixture functions init
// and fini before and after the test is run, respectively.
// The functions init and fini should have type void (*func)(void).
// If init or fini is NULL then that function will *not* be executed.
#define ATEST_RUNF(init, fini, ident) atest__run_(#ident, ident, init, fini)

// The total number of tests that have been run.
ATEST_API int
atest_tests_run(void);

// The total number of tests that have failed.
ATEST_API int
atest_tests_failed(void);

// Internal utilities that must be visible to other header/source files that
// wish to use the atest_* API. Do not use these directly!
// clang-format off
#define ATEST__BLOCK_(code) do { code } while (0)
#define ATEST__IFNOT_(expr, code) ATEST__BLOCK_(if (!(expr)){ code })
ATEST_API void atest__run_(
    char const* name,
    void (*test)(void),
    void (*init)(void),
    void (*fini)(void));
ATEST_API void atest__fail_(char const* file, int line, char const* fmt, ...);
// clang-format on

#endif // ATEST_H_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// IMPLEMENTATION SECTION ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifdef ATEST_IMPLEMENTATION
#undef ATEST_IMPLEMENTATION

static int atest__tests_run_ = 0;
static int atest__tests_failed_ = 0;
static int atest__cur_failed_ = 0;

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

ATEST_API int
atest_tests_run(void)
{
    return atest__tests_run_;
}

ATEST_API int
atest_tests_failed(void)
{
    return atest__tests_failed_;
}

ATEST_API void
atest__run_(
    char const* name,
    void (*test)(void),
    void (*init)(void),
    void (*fini)(void))
{
    assert(name != NULL);
    assert(test != NULL);

    fprintf(stderr, "[TEST     ] :: %s\n", name);
    atest__tests_run_ += 1;
    atest__cur_failed_ = 0;

    if (init != NULL) {
        init();
    }
    test();
    if (fini != NULL) {
        fini();
    }

    char const* const result = atest__cur_failed_ ? "FAIL" : "PASS";
    fprintf(stderr, "[     %s]\n", result);
}

ATEST_API void
atest__fail_(char const* file, int line, char const* fmt, ...)
{
    atest__tests_failed_ += !atest__cur_failed_;
    atest__cur_failed_ = 1;

    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "[%s:%d] ", file, line);
    vfprintf(stderr, fmt, args);
    fputs("\n", stderr);

    va_end(args);
}

#endif // ATEST_IMPLEMENTATION
