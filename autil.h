/*
AUTIL - ASHN'S UTILITY LIBRARY - v0.5.0
    Single header file containing functions and data structures for rapid
    application development in C99 (or later) with a focus on simplicity.
    This library has no dependencies outside of libc.


USAGE
    To create function and object definitions:
        #define AUTIL_IMPLEMENTATION
    in exactly *ONE* translation unit before including this file:
        #define AUTIL_IMPLEMENTATION
        #include "autil.h"

    To restrict function and object definitions to a single translation unit:
        #define AUTIL_API static
    before including this file:
        #define AUTIL_IMPLEMENTATION
        #define AUTIL_API static
        #include "autil.h"

    This library uses the libc functions realloc and free for dynamic memory
    management. If you would like to replace these functions with your own
    custom allocation functions:
        #define AUTIL_REALLOC custom_realloc
        #define AUTIL_FREE custom_free
    before including this file in the translation unit where
    AUTIL_IMPLEMENTATION is defined:
        void* custom_realloc(void* ptr, size_t size);
        void custom_free(void* ptr);
        #define AUTIL_IMPLEMENTATION
        #define AUTIL_REALLOC custom_realloc
        #define AUTIL_FREE custom_free
        #include "autil.h"


LICENSE
    Copyright (c) 2020 ashn <me@ashn.dev>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
    SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
    OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
    CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// HEADER SECTION ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef AUTIL_H_INCLUDED
#define AUTIL_H_INCLUDED

#ifndef AUTIL_API
#    define AUTIL_API extern
#endif

#include <stddef.h> /* size_t, NULL */
#include <stdio.h> /* FILE* */

struct autil_bigint;
struct autil_string;
struct autil_vec;
struct autil_map;

// Produce a pointer of type TYPE* whose contents is the scalar rvalue val.
// This pointer has automatic storage duration associated with the enclosing
// block.
//
// Example:
//      int* pint = AUTIL_LOCAL_PTR(int, 42);
//      char const** pstr = AUTIL_LOCAL_PTR(char const*, "FOO");
//      printf("%d %s\n", *pint, *pstr); // 42 FOO
#define AUTIL_LOCAL_PTR(TYPE, /*val*/...) (&((TYPE){__VA_ARGS__}))

// Dereference ptr as if it were of type TYPE*.
//
// Example:
//      void* ptr = some_func();
//      int val = AUTIL_DEREF_PTR(int, ptr);
#define AUTIL_DEREF_PTR(TYPE, /*ptr*/...) (*(TYPE*)(__VA_ARGS__))

// Unsigned long mask with the n-th bit set.
#define AUTIL_BIT(n) (1UL << (n))

// Number of elements in an array.
#define AUTIL_ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
// Number of characters in a cstring literal, excluding the null-terminator.
#define AUTIL_CSTR_COUNT(cstr_literal) (AUTIL_ARRAY_COUNT(cstr_literal) - 1)
// Number of characters in a formatted string.
#define AUTIL_FMT_COUNT(fmt, ...) ((size_t)snprintf(NULL, 0, fmt, __VA_ARGS__))

// C99 compatible _Alignof operator.
// Produces an integer constant expression.
// clang-format off
#define AUTIL_ALIGNOF(type) offsetof(struct{char _; type ty;}, ty)
// clang-format on

// C99 compatible max_align_t.
// clang-format off
typedef union
{
    _Bool       bool_;
    char        char_;
    short       short_;
    int         int_;
    long        long_;
    long long   long_long_;
    float       float_;
    double      double_;
    long double long_double_;
    void*       void_ptr_;
    void        (*fn_ptr_)();
} autil_max_align_type;
// clang-format on

// C99 compatible(ish) _Static_assert.
// Macro parameter what should be a valid identifier describing the assertion.
// Flips the order of arguments from C11's _Static_assert so that assertions
// read as if they were a sentence.
// Example:
//      // Assert that we are compiling on a 64-bit machine.
//      AUTIL_STATIC_ASSERT(pointers_are_eight_bytes, sizeof(void*) == 8);
// clang-format off
#define AUTIL_STATIC_ASSERT(what, expr)                                        \
    enum {STATIC_ASSERT__ ## what = 1/!!(expr)}//;
// clang-format on

// Should return an int less than, equal to, or greater than zero if lhs is
// semantically less than, equal to, or greater than rhs, respectively.
typedef int (*autil_vpcmp_fn)(void const* lhs, void const* rhs);
// Implementations of autil_vpcmp_fn for builtin types.
AUTIL_API int
autil_void_vpcmp(void const* lhs, void const* rhs); // void (zero-sized object)
AUTIL_API int
autil_cstr_vpcmp(void const* lhs, void const* rhs); // char const*
AUTIL_API int
autil_int_vpcmp(void const* lhs, void const* rhs); // int

// Alternatives to the C99 character handling functions in ctype.h.
// These functions always use the "C" locale and will not result in undefined
// behavior if passed a value not representable by an unsigned char.
// clang-format off
AUTIL_API int autil_isalnum(int c);
AUTIL_API int autil_isalpha(int c);
AUTIL_API int autil_isblank(int c);
AUTIL_API int autil_iscntrl(int c);
AUTIL_API int autil_isdigit(int c);
AUTIL_API int autil_isgraph(int c);
AUTIL_API int autil_islower(int c);
AUTIL_API int autil_isprint(int c);
AUTIL_API int autil_ispunct(int c);
AUTIL_API int autil_isspace(int c);
AUTIL_API int autil_isupper(int c);
AUTIL_API int autil_isbdigit(int c); // Not in C99. Binary digit.
AUTIL_API int autil_isodigit(int c); // Not in C99. Octal digit.
AUTIL_API int autil_isxdigit(int c);
//
AUTIL_API int autil_tolower(int c);
AUTIL_API int autil_toupper(int c);
// clang-format on

// Alternatives to the C99 memory handling functions in string.h.
// These functions do not result in undefined behavior when passed an invalid
// pointer argument paired with a memory-size argument of zero.
// clang-format off
AUTIL_API int autil_memcmp(void const* s1, void const* s2, size_t n);
AUTIL_API void* autil_memmove(void* dest, void const* src, size_t n);
// clang-format on

// Write a formatted info message to stderr.
// A newline is automatically appended to the end of the formatted message.
AUTIL_API void
autil_infof(char const* fmt, ...);
// Write a formatted error message to stderr.
// A newline is automatically appended to the end of the formatted message.
AUTIL_API void
autil_errorf(char const* fmt, ...);
// Write a formatted error message to stderr and exit with EXIT_FAILURE status.
// A newline is automatically appended to the end of the formatted message.
AUTIL_API void
autil_fatalf(char const* fmt, ...);

// General purpose allocator functions with out-of-memory error checking.
// The behavior of autil_xalloc and autil_xallocn is similar to libc realloc and
// *BSD reallocarray with the following exceptions:
// (1) On allocation failure an error message will be printed followed by
//     program termination with EXIT_FAILURE status.
// (2) The call autil_xalloc(ptr, 0) is guaranteed to free the memory backing
//     ptr. A pointer returned by autil_xalloc may be freed with
//     autil_xalloc(ptr, 0) or the equivalent
//     autil_xalloc(ptr, AUTIL_XALLOC_FREE). The calls autil_xallocn(ptr, x, 0)
//     and autil_xallocn(ptr, 0, y) are equivalent to autil_xalloc(ptr, 0).
// The macro AUTIL_XALLOC_FREE may be used in place of the constant zero to
// indicate that a call autil_xalloc(ptr, AUTIL_XALLOC_FREE) is intended as a
// free operation.
AUTIL_API void*
autil_xalloc(void* ptr, size_t size);
//
AUTIL_API void*
autil_xallocn(void* ptr, size_t nmemb, size_t size);
//
#define AUTIL_XALLOC_FREE ((size_t)0)

// Read the full contents of the file specified by path.
// Memory for the read content is allocated with autil_xalloc.
// Returns zero on success.
AUTIL_API int
autil_file_read(char const* path, void** buf, size_t* buf_size);

// Write the contents of a buffer into the file specified by path.
// The file specified by path is created if it does not exist.
// Returns zero on success.
// On failure, the contents of the file specified by path is undefined.
AUTIL_API int
autil_file_write(char const* path, void const* buf, size_t buf_size);

// Read the full contents of the input stream specified by stream.
// Memory for the read content is allocated with autil_xalloc.
// Returns zero on success.
AUTIL_API int
autil_stream_read(FILE* stream, void** buf, size_t* buf_size);

// Read the contents of the input stream specified by stream until a newline or
// end-of-file is encountered.
// The line buffer will *not* have NUL termination.
// The line buffer will contain the end-of-line newline (if present).
// Memory for the read content is allocated with autil_xalloc.
// Returns zero on success.
AUTIL_API int
autil_stream_read_line(FILE* stream, void** buf, size_t* buf_size);

////////////////////////////////////////////////////////////////////////////////
//////// BSTR //////////////////////////////////////////////////////////////////
// Byte string slice.

struct autil_bstr
{
    char const* start;
    size_t count;
};

// Produce a pointer of type struct autil_bstr* constructed from the provided
// parameters. This pointer has automatic storage duration associated with the
// enclosing block.
#define AUTIL_BSTR_LOCAL_PTR(start, count) (&(struct autil_bstr){start, count})
// Create a bstring literal from the provided cstring literal.
#define AUTIL_BSTR_LITERAL(cstr_literal)                                       \
    ((struct autil_bstr){cstr_literal, AUTIL_CSTR_COUNT(cstr_literal)})

// Return an int less than, equal to, or greater than zero if lhs is
// lexicographically less than, equal to, or greater than rhs, respectively.
AUTIL_API int
autil_bstr_cmp(struct autil_bstr const* lhs, struct autil_bstr const* rhs);
// Comparison function satisfying autil_vpcmp_fn.
// Parameters lhs and rhs must be of type struct autil_bstr const*.
AUTIL_API int
autil_bstr_vpcmp(void const* lhs, void const* rhs);

////////////////////////////////////////////////////////////////////////////////
//////// ARR ///////////////////////////////////////////////////////////////////
// General purpose typesafe dynamic array (a.k.a stretchy buffer).
//
// A stretchy buffer works by storing metadata about the number of allocated and
// in-use elements in a header just before the address of the buffer's first
// element. The ith element of a stretchy buffer may be accessed using the array
// index operator, arr[i], and a stretchy buffer containing elements of type T
// may be passed to subroutines as if it were regular array-like pointer of type
// T* or T const*. The address of a stretchy buffer may change when a resizing
// operation is performed, similar to resizing operations done with realloc, so
// the address of a stretchy buffer should not be considered stable.
//
// +--------+--------+--------+--------+--
// | HEADER | ARR[0] | ARR[1] | ARR[2] | ...
// +--------+--------+--------+--------+--
//          ^
//          Pointer manipulated by the user / autil_arr_* macros.
//
// Example:
//      // The declaration:
//      //      TYPE* identifier = NULL;
//      // creates an empty strechy buffer holding TYPE values.
//      int* vals = NULL;
//      printf("count == %zu\n", autil_arr_count(vals));  /* count == 0 */
//
//      for (int i = 0; i < 3; ++i) {
//          autil_arr_push(vals, (i + 1) * 2);
//      }
//      printf("count == %zu\n", autil_arr_count(vals)); /* count == 3 */
//      printf("vals[0] == %d\n", vals[0]); /* vals[0] == 2 */
//      printf("vals[1] == %d\n", vals[1]); /* vals[1] == 4 */
//      printf("vals[2] == %d\n", vals[2]); /* vals[2] == 6 */
//
//      printf("popped == %d\n", autil_arr_pop(vals)); /* popped == 6 */
//      printf("count == %zu\n", autil_arr_count(vals)); /* count == 2 */
//
//      // Free memory allocated to the arr.
//      // This is safe to call even if vals == NULL.
//      autil_arr_fini(vals);

// void autil_arr_fini(TYPE* arr)
// ------------------------------------------------------------
// Free resources associated with the arr.
// Macro parameter arr is evaluated multiple times.
#define autil_arr_fini(arr)                                                    \
    ((void)((arr) != NULL ? AUTIL__ARR_FREE_NON_NULL_HEADER(arr) : NULL))

// size_t autil_arr_count(TYPE* arr)
// ------------------------------------------------------------
// The number of elements in the arr.
// Macro parameter arr is evaluated multiple times.
#define autil_arr_count(arr)                                                   \
    ((size_t)((arr) != NULL ? AUTIL__ARR_PHEADER_CONST_(arr)->cnt_ : (size_t)0))
// size_t autil_arr_capacity(TYPE* arr)
// ------------------------------------------------------------
// The number of elements the allocated in the arr.
// Macro parameter arr is evaluated multiple times.
#define autil_arr_capacity(arr)                                                \
    ((size_t)((arr) != NULL ? AUTIL__ARR_PHEADER_CONST_(arr)->cap_ : (size_t)0))

// void autil_arr_reserve(TYPE* arr, size_t n)
// ------------------------------------------------------------
// Update the minimum capacity of the arr to n elements.
// Macro parameter arr is evaluated multiple times.
#define autil_arr_reserve(arr, n)                                              \
    ((void)((arr) = autil__arr_rsv_(sizeof(*(arr)), arr, n)))
// void autil_arr_resize(TYPE* arr, size_t n)
// ------------------------------------------------------------
// Update the count of the arr to n elements.
// Macro parameter arr is evaluated multiple times.
#define autil_arr_resize(arr, n)                                               \
    ((void)((arr) = autil__arr_rsz_(sizeof(*(arr)), arr, n)))

// void autil_arr_push(TYPE* arr, TYPE val)
// ------------------------------------------------------------
// Append val as the last element of arr.
// Macro parameter arr is evaluated multiple times.
#define autil_arr_push(arr, val)                                               \
    ((void)(AUTIL__ARR_MAYBE_GROW_(arr), AUTIL__ARR_APPEND_VAL_(arr, val)))
// TYPE autil_arr_pop(TYPE* arr)
// ------------------------------------------------------------
// Remove and return the last element of arr.
// This macro does *not* perform bounds checking.
// Macro parameter arr is evaluated multiple times.
#define autil_arr_pop(arr) ((arr)[--AUTIL__ARR_PHEADER_MUTBL_(arr)->cnt_])

// Internal utilities that must be visible to other header/source files that
// wish to use the autil_arr_* API. Do not use these directly!
// clang-format off
struct autil__arr_header_{size_t cnt_; size_t cap_; autil_max_align_type _[];};
enum{AUTIL__ARR_HEADER_OFFSET = sizeof(struct autil__arr_header_)};
#define AUTIL__ARR_PHEADER_MUTBL_(arr_)                                        \
    ((struct autil__arr_header_      *)                                        \
     ((char      *)(arr_)-AUTIL__ARR_HEADER_OFFSET))
#define AUTIL__ARR_PHEADER_CONST_(arr_)                                        \
    ((struct autil__arr_header_ const*)                                        \
     ((char const*)(arr_)-AUTIL__ARR_HEADER_OFFSET))
#define AUTIL__ARR_FREE_NON_NULL_HEADER(arr_)                                  \
    (autil_xalloc(AUTIL__ARR_PHEADER_MUTBL_(arr_), AUTIL_XALLOC_FREE))
#define AUTIL__ARR_MAYBE_GROW_(arr_)                                           \
    ((autil_arr_count(arr_) == autil_arr_capacity(arr_))                       \
         ? (arr_) = autil__arr_grw_(sizeof(*(arr_)), arr_)                     \
         : (arr_))
#define AUTIL__ARR_APPEND_VAL_(arr_, val_)                                     \
    ((arr_)[AUTIL__ARR_PHEADER_MUTBL_(arr_)->cnt_++] = (val_))
AUTIL_API void* autil__arr_rsv_(size_t elemsize, void* arr, size_t cap);
AUTIL_API void* autil__arr_rsz_(size_t elemsize, void* arr, size_t cnt);
AUTIL_API void* autil__arr_grw_(size_t elemsize, void* arr);
// clang-format on

////////////////////////////////////////////////////////////////////////////////
//////// BIG INTEGER ///////////////////////////////////////////////////////////
// Arbitrary precision integer.

extern struct autil_bigint const* const AUTIL_BIGINT_ZERO; // 0
extern struct autil_bigint const* const AUTIL_BIGINT_POS_ONE; // +1
extern struct autil_bigint const* const AUTIL_BIGINT_NEG_ONE; // -1

// Allocate and initialize a bigint to the specified value.
// The call autil_bigint_new(AUTIL_BIGINT_ZERO) will zero-initialize a bigint.
AUTIL_API struct autil_bigint*
autil_bigint_new(struct autil_bigint const* othr);
// Allocate and initialize a bigint from the provided NUL-terminated cstring.
// Returns NULL if the cstring could not be parsed.
//
// The cstring may begin with a plus (+) or minus (-) sign.
// In the absence of a plus or minus sign the cstring will interpreted as a
// non-negative number.
//
// The digits of the cstring may be prefixed with a radix identifier:
// 0b (binary) or 0x (hexadecimal).
// In the absence of a radix identifier, the digits of the cstring will decoded
// with radix 10 (decimal).
//
// The cstring *must* not have any leading or trailing whitespace.
AUTIL_API struct autil_bigint*
autil_bigint_new_cstr(char const* cstr);
// Allocate and initialize a bigint from the provided UTF-8 string.
// Returns NULL if the UTF-8 string could not be parsed.
// This function uses the same string-grammar as autil_bigint_new_cstr().
AUTIL_API struct autil_bigint*
autil_bigint_new_utf8(void const* utf8, size_t utf8_size);
// Deinitialize and free the bigint.
// Does nothing if self == NULL.
AUTIL_API void
autil_bigint_del(struct autil_bigint* self);

// Return an int less than, equal to, or greater than zero if lhs is
// semantically less than, equal to, or greater than rhs, respectively.
AUTIL_API int
autil_bigint_cmp(
    struct autil_bigint const* lhs, struct autil_bigint const* rhs);

// self = othr
AUTIL_API void
autil_bigint_assign(struct autil_bigint* self, struct autil_bigint const* othr);

// self = -1 * self
AUTIL_API void
autil_bigint_negate(struct autil_bigint* self);
// self = abs(self)
AUTIL_API void
autil_bigint_abs(struct autil_bigint* self);
// res = lhs + rhs
AUTIL_API void
autil_bigint_add(
    struct autil_bigint* res,
    struct autil_bigint const* lhs,
    struct autil_bigint const* rhs);
// res = lhs - rhs
AUTIL_API void
autil_bigint_sub(
    struct autil_bigint* res,
    struct autil_bigint const* lhs,
    struct autil_bigint const* rhs);
// res  = lhs * rhs
AUTIL_API void
autil_bigint_mul(
    struct autil_bigint* res,
    struct autil_bigint const* lhs,
    struct autil_bigint const* rhs);
// res  = lhs / rhs
// rem  = lhs % rhs
// If res is NULL then the result will not be written to res.
// If rem is NULL then the remainder will not be written to rem.
//
// This function matches the behavior of the / and % operators as defined by the
// C99 standard, satisfying the expression:
//      (lhs/rhs)*rhs + lhs%rhs == lhs
// where:
//      lhs/rhs == res
//      lhs%rhs == rem
AUTIL_API void
autil_bigint_divrem(
    struct autil_bigint* res,
    struct autil_bigint* rem,
    struct autil_bigint const* lhs,
    struct autil_bigint const* rhs);

// Returns an autil_xalloc-allocated cstring representation of the provided
// bigint using using specified by the format string.
// If fmt is NULL then default formatting is used.
//
// Returns a NUL-terminated string on success.
// Returns NULL if an invalid format string was provided.
//
// Format string grammar: "[flags][width][specifier]"
// Note that the format directive character, %, is *NOT* used in the format
// string grammar.
//
// Flags (optional):
//   #      Prefix the digits of the output string with "0b", "0x", or "0x" when
//          used in conjunction with the "b", "x", or "X" specifiers,
//          respectively. Note that "0x" is used for both the "x" and "X"
//          specifiers.
//   0      Left pad the output string up to the field width using zeros.
//          Default behavior is to pad with spaces.
//   +      Prefix the numeric representation of the output string with a plus
//          or minus sign (+ or -), even for positive numbers.
//          Default behavior is to only add the minus sign for negative numbers.
//   -      Left justify the output string within the provided field width.
//   space  Prefix the numeric representation of the output string with a space
//          if no sign would be written otherwise.
//
// Width (optional):
//   Decimal digit string with nonzero first digit specifying the minimum length
//   of the output string.
//
// Specifier (required):
//   d      The provided bigint will be represented using signed decimal
//          notation.
//   b      The provided bigint will be represented using signed binary
//          notation using sign magnitude representation.
//   x      The provided bigint will be represented using signed hexadecimal
//          notation with *lower case* alphanumeric digits.
//   X      The provided bigint will be represented using signed hexadecimal
//          notation with *UPPER CASE* alphanumeric digits.
AUTIL_API char*
autil_bigint_to_cstr(struct autil_bigint const* self, char const* fmt);

////////////////////////////////////////////////////////////////////////////////
//////// STRING ////////////////////////////////////////////////////////////////
// Byte string with guaranteed NUL termination.

// Allocate and initialize a string from the first count bytes of start.
AUTIL_API struct autil_string*
autil_string_new(char const* start, size_t count);
// Allocate and initialize a string from the provided NUL-terminated cstring.
// If cstr is NULL then string will be initialized to the empty string.
AUTIL_API struct autil_string*
autil_string_new_cstr(char const* cstr);
// Deinitialize and free the string.
// Does nothing if self == NULL.
AUTIL_API void
autil_string_del(struct autil_string* self);

// Pointer to the start of the underlying char array of the string.
// Returns a pointer to a NUL terminator when the count of the string is zero.
AUTIL_API char const*
autil_string_start(struct autil_string const* self);
// The number of bytes in the string *NOT* including the NUL terminator.
AUTIL_API size_t
autil_string_count(struct autil_string const* self);

// Return an int less than, equal to, or greater than zero if lhs is
// lexicographically less than, equal to, or greater than rhs, respectively.
AUTIL_API int
autil_string_cmp(
    struct autil_string const* lhs, struct autil_string const* rhs);

// Update the count of the string.
// If count is greater than the current count of the string then additional
// elements are initialized with garbage data.
AUTIL_API void
autil_string_resize(struct autil_string* self, size_t count);

// Return a pointer to the byte of the string at position idx.
// Fatally exits after printing an error message if idx is out of bounds.
AUTIL_API char*
autil_string_ref(struct autil_string* self, size_t idx);
AUTIL_API char const*
autil_string_ref_const(struct autil_string const* self, size_t idx);

// Insert count bytes of start into the string at position idx.
// Bytes with position greater than idx are moved back count bytes.
// Fatally exits after printing an error message if idx is out of bounds.
AUTIL_API void
autil_string_insert(
    struct autil_string* self, size_t idx, char const* start, size_t count);
// Remove count bytes at position idx from the string.
// Bytes with position greater than idx are moved forward count bytes.
// Fatally exits after printing an error message if the slice to be removed
// indexes out of bounds.
AUTIL_API void
autil_string_remove(struct autil_string* self, size_t idx, size_t count);

// Trim leading and trailing whitespace from the string.
// Bytes of the string are decoded using the "C" locale.
AUTIL_API void
autil_string_trim(struct autil_string* self);

// Split the string on all occurrences of whitespace.
// Empty strings are removed from the result.
// Bytes of the string are decoded using the "C" locale.
// Parameter res will be populated with the collection of resulting strings.
// Example:
//      "A B\tC  D " ===split===> "A" "B" "C"
AUTIL_API void
autil_string_split_to_vec(
    struct autil_string const* self,
    struct autil_vec /* struct autil_string* */* res);
// Split the string on all occurrences of the provided separator.
// Empty strings are *NOT* rmoved from the result.
// Parameter res will be populated with the collection of resulting strings.
//      "ABCBB" ===split on "B"===> "A" "C" "" ""
AUTIL_API void
autil_string_split_to_vec_on(
    struct autil_string const* self,
    char const* separator,
    size_t separator_size,
    struct autil_vec /* struct autil_string* */* res);
AUTIL_API void
autil_string_split_to_vec_on_cstr(
    struct autil_string const* self,
    char const* separator,
    struct autil_vec /* struct autil_string* */* res);

// Wrapper functions for an autil_vec of autil_string*.
// Useful for initializing and deinitializing a vec passed to
// autil_string_split_* functions.
AUTIL_API struct autil_vec /*<struct autil_string*>*/*
autil_vec_of_string_new(void);
AUTIL_API void
autil_vec_of_string_del(struct autil_vec /*<struct autil_string*>*/* vec);

////////////////////////////////////////////////////////////////////////////////
//////// VEC ///////////////////////////////////////////////////////////////////
// General purpose generic resizeable array.
// A vec conceptually consists of the following components:
// (1) data - An array containing the elements of the vec.
// (2) count - The number of elements in the vec.
// (3) capacity - The total number of elements allocated in the data array.
//     This value is always greater than or equal to the count of the vec.

// Allocate and initialize a vec holding elements of size elemsize.
AUTIL_API struct autil_vec*
autil_vec_new(size_t elemsize);
// Deinitialize and free the vec.
// Does nothing if self == NULL.
AUTIL_API void
autil_vec_del(struct autil_vec* self);

// Pointer to the start of the underlying array of the vec.
// May return NULL when the count of the vec is zero.
AUTIL_API void const*
autil_vec_start(struct autil_vec const* self);
// The number of elements in the vec.
AUTIL_API size_t
autil_vec_count(struct autil_vec const* self);
// The number of elements allocated in the vec.
AUTIL_API size_t
autil_vec_capacity(struct autil_vec const* self);
// The sizeof of elements in the vec.
AUTIL_API size_t
autil_vec_elemsize(struct autil_vec const* self);

// Update the minimum capacity of the vec.
// The count of the vec remains unchanged.
AUTIL_API void
autil_vec_reserve(struct autil_vec* self, size_t capacity);
// Update the count of the vec.
// If count is greater than the current count of the vec then additional
// elements are initialized with garbage data.
AUTIL_API void
autil_vec_resize(struct autil_vec* self, size_t count);

// Set the value of the vec *at* position idx to the value *at* data.
// Fatally exits after printing an error message if idx is out of bounds.
//
// Example:
//      struct autil_vec* const v = autil_vec_new(sizeof(int));
//      // some time later...
//      int const foo = 0xdeadbeef;
//      autil_vec_set(v, 42u, &val);
AUTIL_API void
autil_vec_set(struct autil_vec* self, size_t idx, void const* data);
// Get a pointer to the value of the vec *at* position idx.
// Fatally exits after printing an error message if idx is out of bounds.
//
// Example:
//      struct autil_vec* const v = autil_vec_new(sizeof(int));
//      // some time later...
//      int val = AUTIL_DEREF_PTR(int, autil_vec_ref(v, 42u)
AUTIL_API void*
autil_vec_ref(struct autil_vec* self, size_t idx);
AUTIL_API void const*
autil_vec_ref_const(struct autil_vec const* self, size_t idx);

// Insert a copy of the value at data into the vec at position idx.
// Elements with position greater than or equal to idx are moved back one place.
// Fatally exits after printing an error message if idx is greater than the
// count of the vec.
AUTIL_API void
autil_vec_insert(struct autil_vec* self, size_t idx, void const* data);
// Remove the element at position idx from the vec.
// Elements with position greater than idx are moved forward one place.
// If oldelem is not NULL then the removed element will be copied into oldelem.
// Fatally exits after printing an error message if idx is out of bounds.
AUTIL_API void
autil_vec_remove(struct autil_vec* self, size_t idx, void* oldelem);

////////////////////////////////////////////////////////////////////////////////
//////// MAP ///////////////////////////////////////////////////////////////////
// General purpose generic ordered map.
// Maps keys of some key-type to values of some value-type.

// Allocate and initialize a map.
// The map will hold keys of size keysize.
// The map will hold values of size valsize.
// Keys-value pairs in the map are sorted by key using keycmp.
AUTIL_API struct autil_map*
autil_map_new(size_t keysize, size_t valsize, autil_vpcmp_fn keycmp);
// Deinitialize and free the map.
// Does nothing if self == NULL.
AUTIL_API void
autil_map_del(struct autil_map* self);

// The number of key-value pairs in the map.
AUTIL_API size_t
autil_map_count(struct autil_map const* self);
// Reference to the ordered vec of keys in the map.
AUTIL_API struct autil_vec const*
autil_map_keys(struct autil_map const* self);
// Reference to the ordered vec of values in the map.
AUTIL_API struct autil_vec const*
autil_map_vals(struct autil_map const* self);

// Retrieve a pointer to the value associated with key.
// Returns NULL if the map has no key-value pair associated with key.
AUTIL_API void*
autil_map_lookup(struct autil_map* self, void const* key);
AUTIL_API void const*
autil_map_lookup_const(struct autil_map const* self, void const* key);
// Insert a copy of the key-value pair at key/val into the map.
// If a key-value pair with key already exists in the map then the existing key
// and value are replaced with the provided key and value.
//
// If oldkey is not NULL then the removed key will be copied into oldkey.
// If oldval is not NULL then the removed value will be copied into oldval.
// Returns non-zero value if a key-value pair associated with key was replaced
// by the provided key and value.
AUTIL_API int
autil_map_insert(
    struct autil_map* self,
    void const* key,
    void const* val,
    void* oldkey,
    void* oldval);
// Remove the key-value pair associated with key.
// If no such key-value pair exists in the map then the operation is a noop.
//
// If oldkey is not NULL then the removed key will be copied into oldkey.
// If oldval is not NULL then the removed value will be copied into oldval.
// Returns non-zero value if a key-value pair associated with key was removed.
AUTIL_API int
autil_map_remove(
    struct autil_map* self, void const* key, void* oldkey, void* oldval);

#endif // AUTIL_H_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// IMPLEMENTATION SECTION ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifdef AUTIL_IMPLEMENTATION
#undef AUTIL_IMPLEMENTATION

// USE: AUTIL_REALLOC(ptr, size)
#ifndef AUTIL_REALLOC
#    define AUTIL_REALLOC realloc
#endif // AUTIL_REALLOC

// USE: AUTIL_FREE(ptr)
#ifndef AUTIL_FREE
#    define AUTIL_FREE free
#endif // AUTIL_FREE

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

AUTIL_STATIC_ASSERT(CHAR_BIT_IS_8, CHAR_BIT == 8);

AUTIL_API int
autil_void_vpcmp(void const* lhs, void const* rhs)
{
    (void)lhs;
    (void)rhs;
    return 0; // The void (i.e. zero-sized object) type acts as a unit type.
}

AUTIL_API int
autil_cstr_vpcmp(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    return strcmp(*(char const**)lhs, *(char const**)rhs);
}

AUTIL_API int
autil_int_vpcmp(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    int const l = *(int*)lhs;
    int const r = *(int*)rhs;
    if (l < r) {
        return -1;
    }
    if (l > r) {
        return +1;
    }
    return 0;
}

AUTIL_API int
autil_isalnum(int c)
{
    return autil_isalpha(c) || autil_isdigit(c);
}

AUTIL_API int
autil_isalpha(int c)
{
    return autil_isupper(c) || autil_islower(c);
}

AUTIL_API int
autil_isblank(int c)
{
    return c == ' ' || c == '\t';
}

AUTIL_API int
autil_iscntrl(int c)
{
    return (unsigned)c < 0x20 || c == 0x7f;
}

AUTIL_API int
autil_isdigit(int c)
{
    return (unsigned)c - '0' < 10;
}

AUTIL_API int
autil_isgraph(int c)
{
    return autil_isprint(c) && c != ' ';
}

AUTIL_API int
autil_islower(int c)
{
    return (unsigned)c - 'a' < 26;
}

AUTIL_API int
autil_isprint(int c)
{
    return 0x20 <= (unsigned)c && (unsigned)c <= 0x7e;
}

AUTIL_API int
autil_ispunct(int c)
{
    return autil_isgraph(c) && !autil_isalnum(c);
}

AUTIL_API int
autil_isspace(int c)
{
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t'
        || c == '\v';
}

AUTIL_API int
autil_isupper(int c)
{
    return (unsigned)c - 'A' < 26;
}

AUTIL_API int
autil_isbdigit(int c)
{
    return (unsigned)c - '0' < 2;
}

AUTIL_API int
autil_isodigit(int c)
{
    return (unsigned)c - '0' < 8;
}

AUTIL_API int
autil_isxdigit(int c)
{
    return autil_isdigit(c) || (unsigned)c - 'a' < 6 || (unsigned)c - 'A' < 6;
}

AUTIL_API int
autil_tolower(int c)
{
    if (autil_isupper(c)) {
        return c | 0x20;
    }
    return c;
}

AUTIL_API int
autil_toupper(int c)
{
    if (autil_islower(c)) {
        return c & 0x5f;
    }
    return c;
}

AUTIL_API int
autil_memcmp(void const* s1, void const* s2, size_t n)
{
    assert(s1 != NULL || n == 0);
    assert(s2 != NULL || n == 0);

    if (n == 0) {
        return 0;
    }
    return memcmp(s1, s2, n);
}

AUTIL_API void*
autil_memmove(void* dest, void const* src, size_t n)
{
    assert(dest != NULL || n == 0);
    assert(src != NULL || n == 0);

    if (n == 0) {
        return dest;
    }
    return memmove(dest, src, n);
}

AUTIL_API void
autil_infof(char const* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fputs("info: ", stderr);
    vfprintf(stderr, fmt, args);
    fputs("\n", stderr);

    va_end(args);
}

AUTIL_API void
autil_errorf(char const* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fputs("error: ", stderr);
    vfprintf(stderr, fmt, args);
    fputs("\n", stderr);

    va_end(args);
}

AUTIL_API void
autil_fatalf(char const* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fputs("error: ", stderr);
    vfprintf(stderr, fmt, args);
    fputs("\n", stderr);

    va_end(args);
    exit(EXIT_FAILURE);
}

AUTIL_API void*
autil_xalloc(void* ptr, size_t size)
{
    if (size == 0) {
        AUTIL_FREE(ptr);
        return NULL;
    }
    if ((ptr = AUTIL_REALLOC(ptr, size)) == NULL) {
        autil_fatalf("[%s] Out of memory", __func__);
    }
    return ptr;
}

AUTIL_API void*
autil_xallocn(void* ptr, size_t nmemb, size_t size)
{
    size_t const sz = nmemb * size;
    if (nmemb != 0 && sz / nmemb != size) {
        autil_fatalf("[%s] Integer overflow", __func__);
    }
    return autil_xalloc(ptr, sz);
}

// Prepend othr_size bytes from othr onto the autil_xalloc-allocated buffer of
// size *psize pointed to by *pdata, updating the address of *pdata if
// necessary.
static void
autil_xalloc_prepend(
    void** pdata, size_t* psize, void const* othr, size_t othr_size)
{
    assert(pdata != NULL);
    assert(psize != NULL);
    assert(othr != NULL || othr_size == 0);
    if (othr_size == 0) {
        // [] + [A][B][C] => [A][B][C]
        return;
    }

    size_t const new_size = *psize + othr_size;
    void* const new_data = autil_xalloc(*pdata, new_size);
    memmove((char*)new_data + othr_size, new_data, *psize);
    memcpy(new_data, othr, othr_size);

    *pdata = new_data;
    *psize = new_size;
}

// Append othr_size bytes from othr onto the autil_xalloc-allocated buffer of
// size *psize pointed to by *pdata, updating the address of *pdata if
// necessary.
static void
autil_xalloc_append(
    void** pdata, size_t* psize, void const* othr, size_t othr_size)
{
    assert(pdata != NULL);
    assert(psize != NULL);
    assert(othr != NULL || othr_size == 0);
    if (othr_size == 0) {
        // [A][B][C] + [] => [A][B][C]
        return;
    }

    size_t const new_size = *psize + othr_size;
    void* const new_data = autil_xalloc(*pdata, new_size);
    memcpy((char*)new_data + *psize, othr, othr_size);

    *pdata = new_data;
    *psize = new_size;
}

AUTIL_API int
autil_file_read(char const* path, void** buf, size_t* buf_size)
{
    assert(path != NULL);
    assert(buf != NULL);
    assert(buf_size != NULL);

    FILE* const stream = fopen(path, "rb");
    if (stream == NULL) {
        return -1;
    }

    int const err = autil_stream_read(stream, buf, buf_size);
    (void)fclose(stream);

    return err;
}

AUTIL_API int
autil_file_write(char const* path, void const* buf, size_t buf_size)
{
    assert(path != NULL);
    assert(buf != NULL || buf_size == 0);

    FILE* const stream = fopen(path, "wb");
    if (stream == NULL) {
        return -1;
    }

    size_t const written = fwrite(buf, 1, buf_size, stream);
    int const ferr = ferror(stream);
    int const fcls = fclose(stream);

    if (written != buf_size) {
        return -1;
    }
    if (ferr) {
        return -1;
    }
    if (fcls == EOF) {
        // According to the C99 standard:
        // > Any unwritten buffered data for the stream are delivered to the
        // > host environment to be written to the file; any unread buffered
        // > data are discarded. Whether or not the call succeeds, the stream is
        // > disassociated from the file...
        // Cautiously assume that the buffer was not fully written to disk.
        return -1;
    }

    return 0;
}

AUTIL_API int
autil_stream_read(FILE* stream, void** buf, size_t* buf_size)
{
    assert(stream != NULL);
    assert(buf != NULL);
    assert(buf_size != NULL);

    unsigned char* bf = NULL;
    size_t sz = 0;

    int c;
    while ((c = fgetc(stream)) != EOF) {
        bf = autil_xalloc(bf, sz + 1);
        bf[sz++] = (unsigned char)c;
    }
    if (ferror(stream)) {
        autil_xalloc(bf, AUTIL_XALLOC_FREE);
        return -1;
    }

    *buf = bf;
    *buf_size = sz;
    return 0;
}

AUTIL_API int
autil_stream_read_line(FILE* stream, void** buf, size_t* buf_size)
{
    assert(stream != NULL);
    assert(buf != NULL);
    assert(buf_size != NULL);

    unsigned char* bf = NULL;
    size_t sz = 0;

    int c;
    while ((c = fgetc(stream)) != EOF) {
        bf = autil_xalloc(bf, sz + 1);
        bf[sz++] = (unsigned char)c;
        if (c == '\n') {
            break;
        }
    }
    if (ferror(stream)) {
        autil_xalloc(bf, AUTIL_XALLOC_FREE);
        return -1;
    }
    if (feof(stream) && sz == 0) {
        return -1;
    }

    *buf = bf;
    *buf_size = sz;
    return 0;
}

AUTIL_API int
autil_bstr_cmp(struct autil_bstr const* lhs, struct autil_bstr const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    size_t const n = lhs->count < rhs->count ? lhs->count : rhs->count;
    int const cmp = memcmp(lhs->start, rhs->start, n);

    if (cmp != 0 || lhs->count == rhs->count) {
        return cmp;
    }
    return lhs->count < rhs->count ? -1 : +1;
}

AUTIL_API int
autil_bstr_vpcmp(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    return autil_bstr_cmp(lhs, rhs);
}

AUTIL_STATIC_ASSERT(
    ARR_HEADER_OFFSET_IS_ALIGNED,
    AUTIL__ARR_HEADER_OFFSET % AUTIL_ALIGNOF(autil_max_align_type) == 0);

/* reserve */
AUTIL_API void*
autil__arr_rsv_(size_t elemsize, void* arr, size_t cap)
{
    assert(elemsize != 0);

    if (cap <= autil_arr_capacity(arr)) {
        return arr;
    }

    assert(cap != 0);
    size_t const size = AUTIL__ARR_HEADER_OFFSET + elemsize * cap;
    struct autil__arr_header_* const header =
        autil_xalloc(arr != NULL ? AUTIL__ARR_PHEADER_MUTBL_(arr) : NULL, size);
    header->cnt_ = arr != NULL ? header->cnt_ : 0;
    header->cap_ = cap;
    return (char*)header + AUTIL__ARR_HEADER_OFFSET;
}

/* resize */
AUTIL_API void*
autil__arr_rsz_(size_t elemsize, void* arr, size_t cnt)
{
    assert(elemsize != 0);

    if (cnt == 0) {
        autil_arr_fini(arr);
        return NULL;
    }

    if (cnt > autil_arr_capacity(arr)) {
        arr = autil__arr_rsv_(elemsize, arr, cnt);
    }
    assert(arr != NULL);
    AUTIL__ARR_PHEADER_MUTBL_(arr)->cnt_ = cnt;
    return arr;
}

/* grow capacity by doubling */
AUTIL_API void*
autil__arr_grw_(size_t elemsize, void* arr)
{
    assert(elemsize != 0);

    size_t const cap = autil_arr_capacity(arr);
    assert(autil_arr_count(arr) == cap);

    static size_t const GROWTH_FACTOR = 2;
    static size_t const DEFAULT_CAPACITY = 8;
    size_t const new_cap = cap ? cap * GROWTH_FACTOR : DEFAULT_CAPACITY;
    return autil__arr_rsv_(elemsize, arr, new_cap);
}

// The internals of struct autil_bigint are designed such that initializing an
// autil_bigint with:
//      struct autil_bigint foo = {0};
// or
//      struct autil_bigint foo;
//      memset(&foo, 0x00, sizeof(foo));
// will create a bigint equal to zero without requiring heap allocation.
struct autil_bigint
{
    // -1 if the integer <  0
    //  0 if the integer == 0
    // +1 if the integer >  0
    int sign;
    // Magnitude of the integer.
    // Little endian list of bytes.
    // The integer zero will have limbs == NULL.
    uint8_t* limbs;
    // Number of limbs.
    // The integer zero will have count == 0.
    size_t count;
};

struct autil_bigint const* const AUTIL_BIGINT_ZERO =
    &(struct autil_bigint){.sign = 0, .limbs = NULL, .count = 0u};
struct autil_bigint const* const AUTIL_BIGINT_POS_ONE =
    &(struct autil_bigint){.sign = +1, .limbs = (uint8_t[]){0x01}, .count = 1u};
struct autil_bigint const* const AUTIL_BIGINT_NEG_ONE =
    &(struct autil_bigint){.sign = -1, .limbs = (uint8_t[]){0x01}, .count = 1u};

static struct autil_bigint const* const AUTIL_BIGINT_DEC =
    &(struct autil_bigint){.sign = +1, .limbs = (uint8_t[]){0x0A}, .count = 1u};
static struct autil_bigint const* const AUTIL_BIGINT_BIN =
    &(struct autil_bigint){.sign = +1, .limbs = (uint8_t[]){0x02}, .count = 1u};
static struct autil_bigint const* const AUTIL_BIGINT_HEX =
    &(struct autil_bigint){.sign = +1, .limbs = (uint8_t[]){0x10}, .count = 1u};

static void
autil_bigint_fini_(struct autil_bigint* self)
{
    assert(self != NULL);

    autil_xalloc(self->limbs, AUTIL_XALLOC_FREE);
    memset(self, 0x00, sizeof(*self)); // scrub
}

static void
autil_bigint_normalize_(struct autil_bigint* self)
{
    assert(self != NULL);

    while ((self->count != 0) && (self->limbs[self->count - 1] == 0)) {
        self->count -= 1;
    }
    if (self->count == 0) {
        self->sign = 0;
    }
}

// Shift left by nlimbs number of limbs.
// Example:
//      -0xFFEE shifted left by nlimbs=2 becomes -0xFFEE0000 with 8-bit limbs.
static void
autil_bigint_shiftl_(struct autil_bigint* self, size_t nlimbs)
{
    assert(self != NULL);
    if (self->sign == 0) {
        return;
    }

    self->count += nlimbs;
    self->limbs = autil_xallocn(self->limbs, self->count, sizeof(*self->limbs));
    memmove((char*)self->limbs + nlimbs, self->limbs, self->count - nlimbs);
    memset(self->limbs, 0x00, nlimbs);
}

AUTIL_API struct autil_bigint*
autil_bigint_new(struct autil_bigint const* othr)
{
    struct autil_bigint* const self =
        autil_xalloc(NULL, sizeof(struct autil_bigint));
    *self = *AUTIL_BIGINT_ZERO;
    autil_bigint_assign(self, othr);
    return self;
}

AUTIL_API struct autil_bigint*
autil_bigint_new_cstr(char const* cstr)
{
    assert(cstr != NULL);

    return autil_bigint_new_utf8(cstr, strlen(cstr));
}

AUTIL_API struct autil_bigint*
autil_bigint_new_utf8(void const* utf8, size_t utf8_size)
{
    assert(utf8 != NULL);
    struct autil_bigint* self = NULL;
    unsigned char const* const end = (unsigned char const*)utf8 + utf8_size;

    // Default to decimal radix.
    int radix = 10;
    struct autil_bigint const* radix_bigint = AUTIL_BIGINT_DEC;
    int (*radix_isdigit)(int c) = autil_isdigit;

    // Begin iterating over the string from left to right.
    unsigned char const* cur = utf8;
    goto sign;

sign:
    if (cur == end) {
        // No prefix, radix identifier, or digits.
        goto error;
    }
    int sign = +1;
    if (*cur == '+') {
        sign = +1;
        cur += 1;
        goto radix;
    }
    if (*cur == '-') {
        sign = -1;
        cur += 1;
        goto radix;
    }

radix:
    if ((size_t)(end - cur) < AUTIL_CSTR_COUNT("0x")) {
        // String is not long enough to have a radix identifier.
        goto digits;
    }
    if (cur[0] != '0') {
        goto digits;
    }

    if (cur[1] == 'b') {
        radix = 2;
        radix_bigint = AUTIL_BIGINT_BIN;
        radix_isdigit = autil_isbdigit;
        cur += 2;
        goto digits;
    }
    if (cur[1] == 'x') {
        radix = 16;
        radix_bigint = AUTIL_BIGINT_HEX;
        radix_isdigit = autil_isxdigit;
        cur += 2;
        goto digits;
    }

digits:
    if (cur == end) {
        // No digits.
        goto error;
    }
    unsigned char const* const digits_start = cur;
    while (cur != end) {
        if (!radix_isdigit(*cur)) {
            // Invalid digit.
            goto error;
        }
        cur += 1;
    }

    self = autil_bigint_new(AUTIL_BIGINT_ZERO);
    cur = digits_start;
    while (cur != end) {
        errno = 0;
        uint8_t const digit_value =
            (uint8_t)strtol((char[]){(char)*cur, '\0'}, NULL, radix);
        if (errno != 0) {
            goto error;
        }

        struct autil_bigint const digit_bigint = {
            .sign = +1, .limbs = (uint8_t[]){digit_value}, .count = 1u};
        autil_bigint_mul(self, self, radix_bigint);
        autil_bigint_add(self, self, &digit_bigint);

        cur += 1;
    }

    self->sign = sign;
    autil_bigint_normalize_(self);
    return self;

error:
    autil_bigint_del(self);
    return NULL;
}

AUTIL_API void
autil_bigint_del(struct autil_bigint* self)
{
    if (self == NULL) {
        return;
    }

    autil_bigint_fini_(self);
    autil_xalloc(self, AUTIL_XALLOC_FREE);
}

AUTIL_API int
autil_bigint_cmp(struct autil_bigint const* lhs, struct autil_bigint const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    if (lhs->sign > rhs->sign) {
        return +1;
    }
    if (lhs->sign < rhs->sign) {
        return -1;
    }

    assert(lhs->sign == rhs->sign);
    int const sign = lhs->sign;
    if (lhs->count > rhs->count) {
        return sign;
    }
    if (lhs->count < rhs->count) {
        return -sign;
    }

    assert(lhs->count == rhs->count);
    size_t const count = lhs->count;
    for (size_t i = 0; i < count; ++i) {
        size_t limb_idx = count - i - 1;
        uint8_t const lhs_limb = lhs->limbs[limb_idx];
        uint8_t const rhs_limb = rhs->limbs[limb_idx];

        if (lhs_limb > rhs_limb) {
            return sign;
        }
        if (lhs_limb < rhs_limb) {
            return -sign;
        }
    }

    return 0;
}

AUTIL_API void
autil_bigint_assign(struct autil_bigint* self, struct autil_bigint const* othr)
{
    assert(self != NULL);
    assert(othr != NULL);
    if (self == othr) {
        return;
    }

    self->sign = othr->sign;
    self->limbs = autil_xallocn(self->limbs, othr->count, sizeof(*othr->limbs));
    self->count = othr->count;
    if (self->sign != 0) {
        assert(self->limbs != NULL);
        assert(othr->limbs != NULL);
        memcpy(self->limbs, othr->limbs, othr->count * sizeof(*othr->limbs));
    }
}

AUTIL_API void
autil_bigint_negate(struct autil_bigint* self)
{
    assert(self != NULL);

    // +1 * -1 == -1
    // -1 * -1 == +1
    //  0 * -1 ==  0
    self->sign *= -1;
}

AUTIL_API void
autil_bigint_abs(struct autil_bigint* self)
{
    assert(self != NULL);

    // +1 * +1 == +1
    // -1 * -1 == +1
    //  0 *  0 ==  0
    self->sign = self->sign * self->sign;
}

AUTIL_API void
autil_bigint_add(
    struct autil_bigint* res,
    struct autil_bigint const* lhs,
    struct autil_bigint const* rhs)
{
    assert(res != NULL);
    assert(lhs != NULL);
    assert(rhs != NULL);

    // 0 + rhs == rhs
    if (lhs->sign == 0) {
        autil_bigint_assign(res, rhs);
        return;
    }
    // lhs + 0 == lhs
    if (rhs->sign == 0) {
        autil_bigint_assign(res, lhs);
        return;
    }
    // (+lhs) + (-rhs) == (+lhs) - (+rhs)
    if ((lhs->sign == +1) && (rhs->sign == -1)) {
        struct autil_bigint* const RHS = autil_bigint_new(AUTIL_BIGINT_ZERO);
        autil_bigint_assign(RHS, rhs);
        autil_bigint_negate(RHS);
        autil_bigint_sub(res, lhs, RHS);
        autil_bigint_del(RHS);
        return;
    }
    // (-lhs) + (+rhs) == (+rhs) - (+lhs)
    if ((lhs->sign == -1) && (rhs->sign == +1)) {
        struct autil_bigint* const LHS = autil_bigint_new(AUTIL_BIGINT_ZERO);
        autil_bigint_assign(LHS, lhs);
        autil_bigint_negate(LHS);
        autil_bigint_sub(res, rhs, LHS);
        autil_bigint_del(LHS);
        return;
    }

    // (+lhs) + (+rhs) == +(lhs + rhs)
    // (-lhs) + (-rhs) == -(lhs + rhs)
    assert(lhs->sign == rhs->sign);
    int const sign = lhs->sign;

    struct autil_bigint RES = {0};
    RES.sign = sign;
    RES.count = 1 + (lhs->count > rhs->count ? lhs->count : rhs->count);
    RES.limbs = autil_xallocn(RES.limbs, RES.count, sizeof(*RES.limbs));

    unsigned carry = 0;
    for (size_t i = 0; i < RES.count; ++i) {
        unsigned const lhs_limb = i < lhs->count ? lhs->limbs[i] : 0; // upcast
        unsigned const rhs_limb = i < rhs->count ? rhs->limbs[i] : 0; // upcast
        unsigned const tot = lhs_limb + rhs_limb + carry;

        RES.limbs[i] = (uint8_t)tot;
        carry = tot > UINT8_MAX;
    }
    assert(carry == 0);

    autil_bigint_normalize_(&RES);
    autil_bigint_assign(res, &RES);
    autil_bigint_fini_(&RES);
}

AUTIL_API void
autil_bigint_sub(
    struct autil_bigint* res,
    struct autil_bigint const* lhs,
    struct autil_bigint const* rhs)
{
    assert(res != NULL);
    assert(lhs != NULL);
    assert(rhs != NULL);

    // 0 - rhs == -(rhs)
    if (lhs->sign == 0) {
        autil_bigint_assign(res, rhs);
        autil_bigint_negate(res);
        return;
    }
    // lhs - 0 == lhs
    if (rhs->sign == 0) {
        autil_bigint_assign(res, lhs);
        return;
    }
    // (+lhs) - (-rhs) == (+lhs) + (+rhs)
    if ((lhs->sign == +1) && (rhs->sign == -1)) {
        struct autil_bigint* const RHS = autil_bigint_new(AUTIL_BIGINT_ZERO);
        autil_bigint_assign(RHS, rhs);
        autil_bigint_negate(RHS);
        autil_bigint_add(res, lhs, RHS);
        autil_bigint_del(RHS);
        return;
    }
    // (-lhs) - (+rhs) == (-lhs) + (-rhs)
    if ((lhs->sign == -1) && (rhs->sign == +1)) {
        struct autil_bigint* const RHS = autil_bigint_new(AUTIL_BIGINT_ZERO);
        autil_bigint_assign(RHS, rhs);
        autil_bigint_negate(RHS);
        autil_bigint_add(res, lhs, RHS);
        autil_bigint_del(RHS);
        return;
    }

    // (+lhs) - (+rhs) == +(lhs - rhs)
    // (-lhs) - (-rhs) == -(lhs - rhs)
    assert(lhs->sign == rhs->sign);
    int const sign = lhs->sign;
    // Note that the expression (lhs - rhs) will require flipping the sign of
    // the result if the magnitude of lhs is greater than the magnitude of rhs:
    // (+5) - (+3) == +2
    // (+3) - (+5) == -2
    // (-5) - (-3) == -2
    // (-3) - (-5) == +2
    int const cmp = autil_bigint_cmp(lhs, rhs);
    int const neg = ((sign == +1) && (cmp < 0)) || ((sign == -1) && (cmp > 0));
    if (neg) {
        struct autil_bigint const* tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }

    struct autil_bigint RES = {0};
    RES.sign = lhs->sign;
    RES.count = lhs->count > rhs->count ? lhs->count : rhs->count;
    RES.limbs = autil_xallocn(RES.limbs, RES.count, sizeof(*RES.limbs));

    unsigned borrow = 0;
    for (size_t i = 0; i < RES.count; ++i) {
        unsigned const lhs_limb = i < lhs->count ? lhs->limbs[i] : 0; // upcast
        unsigned const rhs_limb = i < rhs->count ? rhs->limbs[i] : 0; // upcast
        unsigned const tot = (lhs_limb - rhs_limb) - borrow;

        RES.limbs[i] = (uint8_t)tot;
        borrow = tot > UINT8_MAX;
    }
    //assert(borrow == 0); // TODO: Check if this right??

    if (neg) {
        autil_bigint_negate(&RES);
    }
    autil_bigint_normalize_(&RES);
    autil_bigint_assign(res, &RES);
    autil_bigint_fini_(&RES);
}

// res  = lhs * rhs
AUTIL_API void
autil_bigint_mul(
    struct autil_bigint* res,
    struct autil_bigint const* lhs,
    struct autil_bigint const* rhs)
{
    assert(res != NULL);
    assert(lhs != NULL);
    assert(rhs != NULL);

    // 0 * rhs == 0
    if (lhs->sign == 0) {
        autil_bigint_assign(res, AUTIL_BIGINT_ZERO);
        return;
    }
    // lhs * 0 == 0
    if (rhs->sign == 0) {
        autil_bigint_assign(res, AUTIL_BIGINT_ZERO);
        return;
    }

    int const sign = lhs->sign * rhs->sign;
    assert(sign == +1 || sign == -1);
    size_t const count = lhs->count + rhs->count;
    assert(count > 0);

    struct autil_bigint RES = {0};
    // Multiplication using partial products.
    // When multiplying the digit at lhs position i by the digit at rhs position
    // j the resulting 2-digit partial product is added at positions i+j (i.e.
    // low digit) and i+j+1 (i.e. high digit).
    //
    //  79 =>  79    79 =>   70     09     70     09
    // *67    *60 + *07     *60 +  *60 +  *07 +  *07
    // ---    ---------    -------------------------
    //                     4200 + 0540 + 0490 + 0063
    //                     ^^      ^^     ^^      ^^
    //                     -------------------------
    //                                          5293
    for (size_t j = 0; j < rhs->count; ++j) {
        for (size_t i = 0; i < lhs->count; ++i) {
            uint16_t const prod = (uint16_t)(lhs->limbs[i] * rhs->limbs[j]);
            uint8_t const prodl = (uint8_t)prod;
            uint8_t const prodh = (uint8_t)(prod >> 8);

            uint8_t* const limbs = autil_xallocn(NULL, count, sizeof(uint8_t));
            memset(limbs, 0x00, count * sizeof(*limbs));

            struct autil_bigint partial;
            partial.sign = sign;
            partial.count = count;
            partial.limbs = limbs;
            partial.limbs[i + j] = prodl;
            partial.limbs[i + j + 1] = prodh;

            autil_bigint_add(&RES, &RES, &partial);
            autil_bigint_fini_(&partial);
        }
    }

    autil_bigint_normalize_(&RES);
    autil_bigint_assign(res, &RES);
    autil_bigint_fini_(&RES);
}

AUTIL_API void
autil_bigint_divrem(
    struct autil_bigint* res,
    struct autil_bigint* rem,
    struct autil_bigint const* lhs,
    struct autil_bigint const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    // 0 / rhs == 0
    if (lhs->sign == 0) {
        if (res != NULL) {
            autil_bigint_assign(res, AUTIL_BIGINT_ZERO);
        }
        if (rem != NULL) {
            autil_bigint_assign(rem, AUTIL_BIGINT_ZERO);
        }
        return;
    }
    // lhs / 0 == undefined
    if (rhs->sign == 0) {
        autil_fatalf("[%s] Divide by zero", __func__);
    }

    struct autil_bigint RES = {0};
    struct autil_bigint REM = {0}; // abs(rem)
    struct autil_bigint RHS = {0}; // abs(rhs)
    autil_bigint_assign(&REM, lhs);
    autil_bigint_abs(&REM);
    autil_bigint_assign(&RHS, rhs);
    autil_bigint_abs(&RHS);

    // Division using goes-into method.
    // In practice the expression (-RHS*NUM) is calculated by repeated
    // subtraction of RHS from TOP a total of NUM times where TOP is composed of
    // as many most-significant digits of REM as required for one iteration of
    // "goes into".
    //
    //               TOP=4      TOP=45    TOP=16
    //               NUM=0      NUM=4     NUM=1
    // RES        ->    0          04        041       041
    //                  ___        ___       ___       ___
    // RHS√REM    -> 11√456  => 11√456 => 11√016 => 11√005 => 41 w/rem 5
    // -RHS*NUM   ->   -0         -44        -11    RHS>REM
    //                  ---        ---       ---
    //                  456        016       005
    size_t ls_idx = REM.count - 1; // Index of the least-significant digit.
    while (autil_bigint_cmp(&REM, &RHS) >= 0) {
        struct autil_bigint TOP = {0};
        autil_bigint_assign(&TOP, &REM);
        memset(TOP.limbs, 0x00, ls_idx);

        struct autil_bigint BOT = {0};
        autil_bigint_assign(&BOT, &RHS);
        autil_bigint_shiftl_(&BOT, ls_idx);

        // Magnitude (MAG).
        // The expression (MAG * NUM) is the partial sum added to the result.
        struct autil_bigint MAG = {0};
        autil_bigint_assign(&MAG, AUTIL_BIGINT_POS_ONE);
        autil_bigint_shiftl_(&MAG, ls_idx);

        // While loop is executed NUM times.
        while (autil_bigint_cmp(&TOP, &BOT) >= 0) {
            autil_bigint_sub(&TOP, &TOP, &BOT);
            autil_bigint_sub(&REM, &REM, &BOT);
            autil_bigint_add(&RES, &RES, &MAG);
        }
        ls_idx -= 1;

        autil_bigint_fini_(&TOP);
        autil_bigint_fini_(&BOT);
        autil_bigint_fini_(&MAG);
    }

    // printf("%2d %2d\n", +7 / +3, +7 % +3); // 2  1
    // printf("%2d %2d\n", +7 / -3, +7 % -3); //-2  1
    // printf("%2d %2d\n", -7 / +3, -7 % +3); //-2 -1
    // printf("%2d %2d\n", -7 / -3, -7 % -3); // 2 -1
    // ISO-IEC-9899-1999 Section 6.5.5 - Multiplicative operators:
    // > When integers are divided, the result of the / operator is the
    // > algebraic quotient with any fractional part discarded. If the quotient
    // > a/b is representable, the expression (a/b)*b + a%b shall equal a.
    RES.sign = lhs->sign * rhs->sign;
    REM.sign = lhs->sign;

    if (res != NULL) {
        autil_bigint_normalize_(&RES);
        autil_bigint_assign(res, &RES);
    }
    if (rem != NULL) {
        autil_bigint_normalize_(&REM);
        autil_bigint_assign(rem, &REM);
    }
    autil_bigint_fini_(&RES);
    autil_bigint_fini_(&REM);
    autil_bigint_fini_(&RHS);
}

// clang-format off
#define AUTIL_BIGINT_FMT_FLAG_HASH_  ((unsigned)0)
#define AUTIL_BIGINT_FMT_FLAG_ZERO_  ((unsigned)1)
#define AUTIL_BIGINT_FMT_FLAG_PLUS_  ((unsigned)2)
#define AUTIL_BIGINT_FMT_FLAG_MINUS_ ((unsigned)3)
#define AUTIL_BIGINT_FMT_FLAG_SPACE_ ((unsigned)4)
// clang-format on
AUTIL_API char*
autil_bigint_to_cstr(struct autil_bigint const* self, char const* fmt)
{
    assert(self != NULL);

    // Parse format string.
    unsigned flags = 0;
    size_t width = 0;
    char specifier = 'd';
    if (fmt != NULL) {
        // Flags
        while (*fmt != '\0' && strchr("#0+- ", *fmt) != NULL) {
            flags |= (unsigned)(*fmt == '#') << AUTIL_BIGINT_FMT_FLAG_HASH_;
            flags |= (unsigned)(*fmt == '0') << AUTIL_BIGINT_FMT_FLAG_ZERO_;
            flags |= (unsigned)(*fmt == '+') << AUTIL_BIGINT_FMT_FLAG_PLUS_;
            flags |= (unsigned)(*fmt == '-') << AUTIL_BIGINT_FMT_FLAG_MINUS_;
            flags |= (unsigned)(*fmt == ' ') << AUTIL_BIGINT_FMT_FLAG_SPACE_;
            fmt += 1;
        }
        // Width
        char* fmt_ =
            (char*)fmt; // Needed due to broken const behavior of strtol.
        if (autil_isdigit(*fmt)) {
            width = (size_t)strtol(fmt, &fmt_, 10);
        }
        fmt = fmt_;
        // Specifier
        char const* whichspec = strchr("dbxX", *fmt);
        if (*fmt == '\0' || whichspec == NULL) {
            return NULL;
        }
        specifier = *whichspec;
        fmt += 1;
        // Invalid trailing digits.
        if (*fmt != '\0') {
            return NULL;
        }
        // Match clang/gcc behavior:
        //      "flag '0' is ignored when flag '-' is present"
        if (flags & AUTIL_BIT(AUTIL_BIGINT_FMT_FLAG_MINUS_)) {
            flags &= (unsigned)~AUTIL_BIT(AUTIL_BIGINT_FMT_FLAG_ZERO_);
        }
    }

    // Prefix.
    void* prefix = NULL;
    size_t prefix_size = 0;
    if (self->sign == +1 && (flags & AUTIL_BIT(AUTIL_BIGINT_FMT_FLAG_PLUS_))) {
        autil_xalloc_append(&prefix, &prefix_size, "+", 1);
    }
    if (self->sign == +1 && (flags & AUTIL_BIT(AUTIL_BIGINT_FMT_FLAG_SPACE_))) {
        autil_xalloc_append(&prefix, &prefix_size, " ", 1);
    }
    if (self->sign == -1) {
        autil_xalloc_append(&prefix, &prefix_size, "-", 1);
    }
    if (flags & AUTIL_BIT(AUTIL_BIGINT_FMT_FLAG_HASH_)) {
        if (specifier == 'b') {
            autil_xalloc_append(&prefix, &prefix_size, "0b", 2);
        }
        if (specifier == 'x') {
            autil_xalloc_append(&prefix, &prefix_size, "0x", 2);
        }
        if (specifier == 'X') {
            autil_xalloc_append(&prefix, &prefix_size, "0x", 2);
        }
    }

    // Digits.
    void* digits = NULL;
    size_t digits_size = 0;
    char digit_buf[8u + AUTIL_CSTR_COUNT("\0")] = {0};
    if (specifier == 'd') {
        struct autil_bigint DEC = {0};
        struct autil_bigint SELF = {0};
        autil_bigint_assign(&SELF, self);
        autil_bigint_abs(&SELF);
        while (autil_bigint_cmp(&SELF, AUTIL_BIGINT_ZERO) != 0) {
            autil_bigint_divrem(&SELF, &DEC, &SELF, AUTIL_BIGINT_DEC);
            assert(DEC.count <= 1);
            assert(DEC.limbs == NULL || DEC.limbs[0] < 10);
            sprintf(digit_buf, "%d", DEC.limbs != NULL ? (int)DEC.limbs[0] : 0);
            autil_xalloc_prepend(
                &digits, &digits_size, digit_buf, strlen(digit_buf));
        }
        autil_bigint_fini_(&DEC);
        autil_bigint_fini_(&SELF);
    }
    else if (specifier == 'b') {
        for (size_t i = self->count - 1; i < self->count; --i) {
            sprintf(
                digit_buf,
                "%c%c%c%c%c%c%c%c",
                ((int)self->limbs[i] >> 7) & 0x01 ? '1' : '0',
                ((int)self->limbs[i] >> 6) & 0x01 ? '1' : '0',
                ((int)self->limbs[i] >> 5) & 0x01 ? '1' : '0',
                ((int)self->limbs[i] >> 4) & 0x01 ? '1' : '0',
                ((int)self->limbs[i] >> 3) & 0x01 ? '1' : '0',
                ((int)self->limbs[i] >> 2) & 0x01 ? '1' : '0',
                ((int)self->limbs[i] >> 1) & 0x01 ? '1' : '0',
                ((int)self->limbs[i] >> 0) & 0x01 ? '1' : '0');
            autil_xalloc_append(
                &digits, &digits_size, digit_buf, strlen(digit_buf));
        }
    }
    else if (specifier == 'x') {
        for (size_t i = self->count - 1; i < self->count; --i) {
            sprintf(digit_buf, "%02x", (int)self->limbs[i]);
            autil_xalloc_append(
                &digits, &digits_size, digit_buf, strlen(digit_buf));
        }
    }
    else if (specifier == 'X') {
        for (size_t i = self->count - 1; i < self->count; --i) {
            sprintf(digit_buf, "%02X", (int)self->limbs[i]);
            autil_xalloc_append(
                &digits, &digits_size, digit_buf, strlen(digit_buf));
        }
    }
    else {
        autil_fatalf("Unreachable!");
    }

    if (digits_size != 0) {
        // Remove leading zeros.
        size_t z = 0;
        while (z < digits_size && (((char*)digits)[z] == '0')) {
            z += 1;
        }
        digits_size -= z;
        memmove(digits, (char*)digits + z, digits_size);
    }
    else {
        // The number zero contains one digit - zero.
        autil_xalloc_append(&digits, &digits_size, "0", 1);
    }

    // Width.
    void* widths = NULL;
    size_t widths_size = 0;
    if ((prefix_size + digits_size) < width) {
        char pad = ' ';
        if (flags & AUTIL_BIT(AUTIL_BIGINT_FMT_FLAG_ZERO_)) {
            assert(!(flags & AUTIL_BIT(AUTIL_BIGINT_FMT_FLAG_MINUS_)));
            pad = '0';
        }
        widths_size = width - (prefix_size + digits_size);
        widths = autil_xalloc(widths, widths_size);
        memset(widths, pad, widths_size);

        if (flags & AUTIL_BIT(AUTIL_BIGINT_FMT_FLAG_ZERO_)) {
            assert(!(flags & AUTIL_BIT(AUTIL_BIGINT_FMT_FLAG_MINUS_)));
            autil_xalloc_prepend(&digits, &digits_size, widths, widths_size);
        }
        else if (flags & AUTIL_BIT(AUTIL_BIGINT_FMT_FLAG_MINUS_)) {
            assert(!(flags & AUTIL_BIT(AUTIL_BIGINT_FMT_FLAG_ZERO_)));
            autil_xalloc_append(&digits, &digits_size, widths, widths_size);
        }
        else {
            autil_xalloc_prepend(&prefix, &prefix_size, widths, widths_size);
        }
    }

    void* cstr = NULL;
    size_t cstr_size = 0;
    autil_xalloc_append(&cstr, &cstr_size, prefix, prefix_size);
    autil_xalloc_append(&cstr, &cstr_size, digits, digits_size);
    autil_xalloc_append(&cstr, &cstr_size, "\0", 1);
    autil_xalloc(prefix, AUTIL_XALLOC_FREE);
    autil_xalloc(digits, AUTIL_XALLOC_FREE);
    autil_xalloc(widths, AUTIL_XALLOC_FREE);
    return cstr;
}

// This function is "technically" part of the public API, but it is not given a
// prototype, and is intended for debugging purposes only.
// Expect this function to be removed in future versions of this header.
AUTIL_API void
autil_bigint_dump(struct autil_bigint const* self)
{
    int sign = self->sign;
    char signc = '0';
    if (sign == +1) {
        signc = '+';
    }
    if (sign == -1) {
        signc = '-';
    }

    FILE* const fp = stdout;
    fprintf(fp, "SIGN: '%c', COUNT: %zu, LIMBS: [", signc, self->count);
    for (size_t i = 0; i < self->count; ++i) {
        fprintf(fp, "0x%02X", (int)self->limbs[i]);
        if (i != self->count - 1) {
            fputs(", ", fp);
        }
    }
    fputs("]\n", fp);
}

struct autil_string
{
    char* start;
    size_t count;
};
#define AUTIL_STRING_SIZE_(count_) (count_ + AUTIL_CSTR_COUNT("\0"))

AUTIL_API struct autil_string*
autil_string_new(char const* start, size_t count)
{
    assert(start != NULL || count == 0);

    struct autil_string* const self =
        autil_xalloc(NULL, sizeof(struct autil_string));

    self->start = autil_xalloc(NULL, AUTIL_STRING_SIZE_(count));
    self->count = count;

    if (start != NULL) {
        memcpy(self->start, start, count);
    }
    self->start[self->count] = '\0';

    return self;
}

AUTIL_API struct autil_string*
autil_string_new_cstr(char const* cstr)
{
    if (cstr == NULL) {
        cstr = "";
    }
    return autil_string_new(cstr, strlen(cstr));
}

AUTIL_API void
autil_string_del(struct autil_string* self)
{
    if (self == NULL) {
        return;
    }

    autil_xalloc(self->start, AUTIL_XALLOC_FREE);
    memset(self, 0x00, sizeof(*self)); // scrub
    autil_xalloc(self, AUTIL_XALLOC_FREE);
}

AUTIL_API char const*
autil_string_start(struct autil_string const* self)
{
    assert(self != NULL);

    return self->start;
}

AUTIL_API size_t
autil_string_count(struct autil_string const* self)
{
    assert(self != NULL);

    return self->count;
}

AUTIL_API int
autil_string_cmp(struct autil_string const* lhs, struct autil_string const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    size_t const n = lhs->count < rhs->count ? lhs->count : rhs->count;
    int const cmp = memcmp(lhs->start, rhs->start, n);

    if (cmp != 0 || lhs->count == rhs->count) {
        return cmp;
    }
    return lhs->count < rhs->count ? -1 : +1;
}

AUTIL_API void
autil_string_resize(struct autil_string* self, size_t count)
{
    assert(self != NULL);

    if (count > self->count) {
        self->start = autil_xalloc(self->start, AUTIL_STRING_SIZE_(count));
        char* const fill_start = self->start + AUTIL_STRING_SIZE_(self->count);
        size_t const fill_count = count - self->count;
        memset(fill_start, 0x00, fill_count); // Fill new space with NULs.
    }
    self->count = count;
    self->start[self->count] = '\0';
}

AUTIL_API char*
autil_string_ref(struct autil_string* self, size_t idx)
{
    assert(self != NULL);

    if (idx >= self->count) {
        autil_fatalf("[%s] Index out of bounds (%zu)", __func__, idx);
    }
    return &self->start[idx];
}

AUTIL_API char const*
autil_string_ref_const(struct autil_string const* self, size_t idx)
{
    assert(self != NULL);

    if (idx >= self->count) {
        autil_fatalf("[%s] Index out of bounds (%zu)", __func__, idx);
    }
    return &self->start[idx];
}

AUTIL_API void
autil_string_insert(
    struct autil_string* self, size_t idx, char const* start, size_t count)
{
    assert(self != NULL);
    assert(start != NULL || count == 0);
    if (idx > self->count) {
        autil_fatalf("[%s] Invalid index %zu", __func__, idx);
    }

    if (count == 0) {
        return;
    }
    size_t const mov_count = self->count - idx;
    autil_string_resize(self, self->count + count);
    memmove(self->start + idx + count, self->start + idx, mov_count);
    memmove(self->start + idx, start, count);
}

AUTIL_API void
autil_string_remove(struct autil_string* self, size_t idx, size_t count)
{
    assert(self != NULL);
    if ((idx + count) > self->count) {
        autil_fatalf("[%s] Invalid index,count %zu,%zu", __func__, idx, count);
    }

    if (count == 0) {
        return;
    }
    memmove(self->start + idx, self->start + idx + count, self->count - count);
    autil_string_resize(self, self->count - count);
}

AUTIL_API void
autil_string_trim(struct autil_string* self)
{
    assert(self != NULL);
    size_t n;

    // Trim leading characters.
    n = 0;
    while (n < self->count && autil_isspace(self->start[n])) {
        n += 1;
    }
    if (n != 0) {
        autil_string_remove(self, 0, n);
    }

    // Trim trailing characters.
    n = self->count;
    while (n > 0 && autil_isspace(self->start[n - 1])) {
        n -= 1;
    }
    autil_string_resize(self, n);
}

// Should be equivalent to str.split(sep=None) from Python3.
AUTIL_API void
autil_string_split_to_vec(
    struct autil_string const* self, struct autil_vec* res)
{
    assert(self != NULL);
    assert(res != NULL);
    assert(autil_vec_elemsize(res) == sizeof(struct autil_string*));

    autil_vec_resize(res, 0);

    size_t first = 0;
    while (first < self->count) {
        if (autil_isspace(self->start[first])) {
            first += 1;
            continue;
        }

        size_t end = first;
        while (end < self->count && !autil_isspace(self->start[end])) {
            end += 1;
        }
        struct autil_string* const s =
            autil_string_new(self->start + first, end - first);
        autil_vec_insert(res, autil_vec_count(res), &s);
        first = end;
    }
}

AUTIL_API void
autil_string_split_to_vec_on(
    struct autil_string const* self,
    char const* separator,
    size_t separator_size,
    struct autil_vec* res)
{
    assert(self != NULL);
    assert(res != NULL);
    assert(autil_vec_elemsize(res) == sizeof(struct autil_string*));

    autil_vec_resize(res, 0);
    if (separator_size == 0) {
        struct autil_string* const s =
            autil_string_new(self->start, self->count);
        autil_vec_insert(res, autil_vec_count(res), &s);
        return;
    }

    char const* const end_of_string = self->start + self->count;
    char const* beg = self->start;
    char const* end = beg;
    while ((size_t)(end_of_string - end) >= separator_size) {
        if (memcmp(end, separator, separator_size) != 0) {
            end += 1;
            continue;
        }
        struct autil_string* const s =
            autil_string_new(beg, (size_t)(end - beg));
        autil_vec_insert(res, autil_vec_count(res), &s);
        beg = end + separator_size;
        end = beg;
    }
    struct autil_string* const s =
        autil_string_new(beg, (size_t)(end_of_string - beg));
    autil_vec_insert(res, autil_vec_count(res), &s);
}

AUTIL_API void
autil_string_split_to_vec_on_cstr(
    struct autil_string const* self,
    char const* separator,
    struct autil_vec* res)
{
    assert(self != NULL);
    assert(res != NULL);
    assert(autil_vec_elemsize(res) == sizeof(struct autil_string*));

    autil_string_split_to_vec_on(self, separator, strlen(separator), res);
}

AUTIL_API struct autil_vec*
autil_vec_of_string_new(void)
{
    return autil_vec_new(sizeof(struct autil_string*));
}

AUTIL_API void
autil_vec_of_string_del(struct autil_vec* vec)
{
    for (size_t i = 0; i < autil_vec_count(vec); ++i) {
        struct autil_string** const ref = autil_vec_ref(vec, i);
        autil_string_del(*ref);
    }
    autil_vec_del(vec);
}

struct autil_vec
{
    void* start;
    size_t count;
    size_t capacity;
    size_t elemsize;
};

AUTIL_API struct autil_vec*
autil_vec_new(size_t elemsize)
{
    struct autil_vec* const self = autil_xalloc(NULL, sizeof(struct autil_vec));
    self->start = NULL;
    self->count = 0;
    self->capacity = 0;
    self->elemsize = elemsize;
    return self;
}

AUTIL_API void
autil_vec_del(struct autil_vec* self)
{
    if (self == NULL) {
        return;
    }

    autil_xalloc(self->start, AUTIL_XALLOC_FREE);
    memset(self, 0x00, sizeof(*self)); // scrub
    autil_xalloc(self, AUTIL_XALLOC_FREE);
}

AUTIL_API void const*
autil_vec_start(struct autil_vec const* self)
{
    assert(self != NULL);

    return self->start;
}

AUTIL_API size_t
autil_vec_count(struct autil_vec const* self)
{
    assert(self != NULL);

    return self->count;
}

AUTIL_API size_t
autil_vec_capacity(struct autil_vec const* self)
{
    assert(self != NULL);

    return self->capacity;
}

AUTIL_API size_t
autil_vec_elemsize(struct autil_vec const* self)
{
    assert(self != NULL);

    return self->elemsize;
}

AUTIL_API void
autil_vec_reserve(struct autil_vec* self, size_t capacity)
{
    assert(self != NULL);

    if (capacity <= self->capacity) {
        return;
    }

    self->start = autil_xallocn(self->start, capacity, self->elemsize);
    self->capacity = capacity;
}

AUTIL_API void
autil_vec_resize(struct autil_vec* self, size_t count)
{
    assert(self != NULL);

    if (count > self->capacity) {
        autil_vec_reserve(self, count);
    }

    self->count = count;
}

AUTIL_API void
autil_vec_set(struct autil_vec* self, size_t idx, void const* data)
{
    assert(self != NULL);

    if (idx >= self->count) {
        autil_fatalf("[%s] Index out of bounds (%zu)", __func__, idx);
    }
    if (self->elemsize == 0) {
        return;
    }

    void* const ref = ((char*)self->start) + (idx * self->elemsize);
    memmove(ref, data, self->elemsize);
}

AUTIL_API void*
autil_vec_ref(struct autil_vec* self, size_t idx)
{
    assert(self != NULL);

    if (idx >= self->count) {
        autil_fatalf("[%s] Index out of bounds (%zu)", __func__, idx);
    }
    return ((char*)self->start) + (idx * self->elemsize);
}

AUTIL_API void const*
autil_vec_ref_const(struct autil_vec const* self, size_t idx)
{
    assert(self != NULL);

    if (idx >= self->count) {
        autil_fatalf("[%s] Index out of bounds (%zu)", __func__, idx);
    }
    return ((char*)self->start) + (idx * self->elemsize);
}

AUTIL_API void
autil_vec_insert(struct autil_vec* self, size_t idx, void const* data)
{
    assert(self != NULL);

    if (idx > self->count) {
        autil_fatalf("[%s] Invalid index %zu", __func__, idx);
    }
    if (self->elemsize == 0) {
        self->count += 1;
        return;
    }

    // [A][B][C][D][E]
    // [A][B][C][D][E][ ]
    if (self->count == self->capacity) {
        static size_t const GROWTH_FACTOR = 2;
        static size_t const DEFAULT_CAPACITY = 32;
        size_t const capacity =
            self->count ? self->count * GROWTH_FACTOR : DEFAULT_CAPACITY;
        autil_vec_reserve(self, capacity);
    }

    // [A][B][C][D][E][ ]
    // [A][B][ ][C][D][E]
    size_t const move_count = self->count - idx;
    size_t const move_size = move_count * self->elemsize;
    void* const move_src = ((char*)self->start) + (idx * self->elemsize);
    void* const move_dst = ((char*)move_src) + 1 * self->elemsize;
    memmove(move_dst, move_src, move_size);
    self->count += 1;

    // [A][B][ ][C][D][E]
    // [A][B][X][C][D][E]
    autil_vec_set(self, idx, data);
}

AUTIL_API void
autil_vec_remove(struct autil_vec* self, size_t idx, void* oldelem)
{
    assert(self != NULL);
    if (idx >= self->count) {
        autil_fatalf("[%s] Invalid index %zu", __func__, idx);
    }
    if (self->elemsize == 0) {
        self->count -= 1;
        return;
    }

    if (oldelem != NULL) {
        memcpy(oldelem, autil_vec_ref(self, idx), self->elemsize);
    }

    // [A][B][X][C][D][E]
    // [A][B][C][D][E][ ]
    size_t const move_count = (self->count - 1) - idx;
    size_t const move_size = move_count * self->elemsize;
    void* const move_dst = ((char*)self->start) + (idx * self->elemsize);
    void* const move_src = ((char*)move_dst) + 1 * self->elemsize;
    memmove(move_dst, move_src, move_size);
    self->count -= 1;
}

struct autil_map
{
    struct autil_vec* keys;
    struct autil_vec* vals;
    autil_vpcmp_fn keycmp;
};

AUTIL_API struct autil_map*
autil_map_new(size_t keysize, size_t valsize, autil_vpcmp_fn keycmp)
{
    struct autil_map* const self = autil_xalloc(NULL, sizeof(struct autil_map));
    self->keys = autil_vec_new(keysize);
    self->vals = autil_vec_new(valsize);
    self->keycmp = keycmp;
    return self;
}

AUTIL_API void
autil_map_del(struct autil_map* self)
{
    if (self == NULL) {
        return;
    }

    autil_vec_del(self->keys);
    autil_vec_del(self->vals);
    memset(self, 0x00, sizeof(*self)); // scrub
    autil_xalloc(self, AUTIL_XALLOC_FREE);
}

AUTIL_API size_t
autil_map_count(struct autil_map const* self)
{
    assert(self != NULL);

    return autil_vec_count(self->keys);
}

AUTIL_API struct autil_vec const*
autil_map_keys(struct autil_map const* self)
{
    assert(self != NULL);

    return self->keys;
}

AUTIL_API struct autil_vec const*
autil_map_vals(struct autil_map const* self)
{
    assert(self != NULL);

    return self->vals;
}

// Returns the (positive) index of key if it exists in self.
// Returns a negative number that, if negated and subtracting one from, would be
// the index of key if inserted (I.E -1 means insert at 0 and -42 means insert
// at 41).
static long
autil_map_find_(struct autil_map const* self, void const* key)
{
    assert(self != NULL);
    assert(autil_map_count(self) <= LONG_MAX);

    if (autil_map_count(self) == 0) {
        return -1;
    }

    long bot = 0;
    long top = (long)autil_map_count(self) - 1;
    long mid;
    while (bot <= top) {
        mid = (bot + top) / 2;
        void* const midkey = autil_vec_ref(self->keys, (size_t)mid);
        int const cmp = self->keycmp(midkey, key);
        if (cmp == 0) {
            return mid;
        }
        if (cmp < 0) {
            bot = mid + 1;
            continue;
        }
        if (cmp > 0) {
            top = mid - 1;
            continue;
        }
        autil_fatalf("Unreachable!");
    }

    return (-1 * bot) - 1;
}

AUTIL_API void*
autil_map_lookup(struct autil_map* self, void const* key)
{
    assert(self != NULL);

    long const location = autil_map_find_(self, key);
    if (location < 0) {
        return NULL;
    }
    return autil_vec_ref(self->vals, (size_t)location);
}

AUTIL_API void const*
autil_map_lookup_const(struct autil_map const* self, void const* key)
{
    assert(self != NULL);

    long const location = autil_map_find_(self, key);
    if (location < 0) {
        return NULL;
    }
    return autil_vec_ref(self->vals, (size_t)location);
}

AUTIL_API int
autil_map_insert(
    struct autil_map* self,
    void const* key,
    void const* val,
    void* oldkey,
    void* oldval)
{
    assert(self != NULL);

    long const location = autil_map_find_(self, key);
    if (location < 0) {
        size_t const idx = (size_t)(-1 * location) - 1;
        autil_vec_insert(self->keys, idx, key);
        autil_vec_insert(self->vals, idx, val);
        return 0;
    }

    size_t const idx = (size_t)location;
    if (oldkey != NULL) {
        memcpy(oldkey, autil_vec_ref(self->keys, idx), self->keys->elemsize);
    }
    if (oldval != NULL) {
        memcpy(oldval, autil_vec_ref(self->vals, idx), self->vals->elemsize);
    }
    autil_vec_set(self->keys, idx, key);
    autil_vec_set(self->vals, idx, val);
    return 1;
}

AUTIL_API int
autil_map_remove(
    struct autil_map* self, void const* key, void* oldkey, void* oldval)
{
    assert(self != NULL);

    long const location = autil_map_find_(self, key);
    if (location < 0) {
        return 0;
    }

    size_t const idx = (size_t)location;
    autil_vec_remove(self->keys, idx, oldkey);
    autil_vec_remove(self->vals, idx, oldval);
    return 1;
}

#endif // AUTIL_IMPLEMENTATION
