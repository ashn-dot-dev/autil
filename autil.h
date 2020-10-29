/*
AUTIL - ASHN'S UTILITY LIBRARY
    Single header file containing functions and data structures for rapid
    application development in C99 (or later) with a focus on simplicity.
    This library has no additional dependencies outside of libc.


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


CHANGELOG
    Unreleased
    -------------------
    + Namespace library with AUTIL_ and autil_.
    + Added macro AUTIL_FMT_COUNT.
    + Added safe ctype functions: autil_isalnum, autil_isalpha, autil_isblank,
      autil_iscntrl, autil_isdigit, autil_isgraph, autil_islower, autil_isprint,
      autil_ispunct, autil_isspace, autil_isupper, autil_isxdigit,
      autil_tolower, and autil_toupper.
    + Added function autil_vec_elemsize.

    v0.2.0 - 2020-09-16
    -------------------
    + Added function infof.
    + Added macro ARRAY_COUNT.
    + Added macro CSTR_COUNT.
    + Fixed missing const qualifier for vec_get parameter self.

    v0.1.0 - 2020-07-30
    -------------------
    + Initial release.
    + Added type vpcmp_fn and functions str_vpcmp and int_vpcmp.
    + Added macros LOCAL_PTR and DEREF_PTR.
    + Added functions errorf and fatalf.
    + Added functions xalloc and xallocn and macro XALLOC_FREE.
    + Added functions file_read and file_write.
    + Added type vec and associated functions: vec_new, vec_del, vec_count,
      vec_capacity, vec_reserve, vec_resize, vec_set, vec_get, vec_insert, and
      vec_remove.
    + Added type map and associated functions: map_new, map_del, map_count,
      map_keys, map_vals, map_lookup, map_insert, and map_remove.


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

#include <stddef.h>

// Should return an int less than, equal to, or greater than zero if lhs is
// semantically less than, equal to, or greater than rhs, respectively.
typedef int (*autil_vpcmp_fn)(void const* lhs, void const* rhs);
// Implementations of autil_vpcmp_fn for builtin types.
AUTIL_API int
autil_str_vpcmp(void const* lhs, void const* rhs); // char const*
AUTIL_API int
autil_int_vpcmp(void const* lhs, void const* rhs); // int

// Produce a pointer of type TYPE* whose contents is the scalar rvalue val.
// This pointer has automatic storage duration associated with the enclosing
// block.
//
// Example:
//  int* pint = AUTIL_LOCAL_PTR(int, 42);
//  char const** pstr = AUTIL_LOCAL_PTR(char const*, "FOO");
//  printf("%d %s\n", *pint, *pstr); // 42 FOO
#define AUTIL_LOCAL_PTR(TYPE, /*val*/...) (&((TYPE){__VA_ARGS__}))

// Dereference ptr as if it were of type TYPE*.
//
// Example:
//  void* ptr = some_func();
//  int val = AUTIL_DEREF_PTR(int, ptr);
#define AUTIL_DEREF_PTR(TYPE, /*ptr*/...) (*(TYPE*)(__VA_ARGS__))

// Number of elements in an array.
#define AUTIL_ARRAY_COUNT(array_) (sizeof(array_) / sizeof((array_)[0]))
// Number of characters in a cstring literal, excluding the null-terminator.
#define AUTIL_CSTR_COUNT(cstr_) (AUTIL_ARRAY_COUNT(cstr_) - 1)

// Number of characters in a formatted string.
#define AUTIL_FMT_COUNT(fmt, ...) ((size_t)snprintf(NULL, 0, fmt, __VA_ARGS__))

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
AUTIL_API int autil_isxdigit(int c);
//
AUTIL_API int autil_tolower(int c);
AUTIL_API int autil_toupper(int c);
// clang-format on

// Write a formatted error message to stderr.
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
// Returns zero on success, in which case a pointer to the
// autil_xalloc-allocated buffer and that buffer's size are stored in *buf and
// *buf_size.
AUTIL_API int
autil_file_read(char const* path, void** buf, size_t* buf_size);

// Write the contents of a buffer into the file specified by path.
// The file specified by path is created if it does not exist.
// Returns zero on success.
// On failure, the contents of the file specified by path is undefined.
AUTIL_API int
autil_file_write(char const* path, void const* buf, size_t buf_size);

////////////////////////////////////////////////////////////////////////////////
//////// VEC ///////////////////////////////////////////////////////////////////

// General purpose generic resizeable array.
// A vec conceptually consists of the following components:
// (1) data - An array containing the elements of the vec.
// (2) count - The number of elements in the vec.
// (3) capacity - The total number of elements allocated in the data array.
//     This value is always greater than or equal to the count of the vec.
struct autil_vec;

// Allocate and initialize a vec holding elements of size elemsize.
AUTIL_API struct autil_vec*
autil_vec_new(size_t elemsize);
// Deinitialize and free the vec.
AUTIL_API void
autil_vec_del(struct autil_vec* self);

// Pointer to the start of the underlying array of the vec.
// May return NULL when the count of the vec is zero.
AUTIL_API void*
autil_vec_data(struct autil_vec const* self);
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

// Set the value of the vec at position idx to the value at data.
// Fatally exits after printing an error message if idx is out of bounds.
AUTIL_API void
autil_vec_set(struct autil_vec* self, size_t idx, void const* data);
// Get a pointer to the value of the vec at position idx.
// Fatally exits after printing an error message if idx is out of bounds.
AUTIL_API void*
autil_vec_get(struct autil_vec const* self, size_t idx);

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
struct autil_map;

// Allocate and initialize a map.
// The map will hold keys of size keysize.
// The map will hold values of size valsize.
// Keys-value pairs in the map are sorted by key using keycmp.
AUTIL_API struct autil_map*
autil_map_new(size_t keysize, size_t valsize, autil_vpcmp_fn keycmp);
// Deinitialize and free the map.
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
autil_map_lookup(struct autil_map const* self, void const* key);
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

#ifndef AUTIL_REALLOC
// USE: AUTIL_REALLOC(ptr, size)
#    define AUTIL_REALLOC realloc
#endif // AUTIL_REALLOC

#ifndef AUTIL_FREE
// USE: AUTIL_FREE(ptr)
#    define AUTIL_FREE free
#endif // AUTIL_FREE

#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AUTIL_API int
autil_str_vpcmp(void const* lhs, void const* rhs)
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

    (void)fclose(stream);
    *buf = bf;
    *buf_size = sz;
    return 0;
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

struct autil_vec
{
    void* data;
    size_t count;
    size_t capacity;
    size_t elemsize;
};

AUTIL_API struct autil_vec*
autil_vec_new(size_t elemsize)
{
    struct autil_vec* const self = autil_xalloc(NULL, sizeof(struct autil_vec));
    self->data = NULL;
    self->count = 0;
    self->capacity = 0;
    self->elemsize = elemsize;
    return self;
}

AUTIL_API void
autil_vec_del(struct autil_vec* self)
{
    assert(self != NULL);

    autil_xalloc(self->data, AUTIL_XALLOC_FREE);
    memset(self, 0x00, sizeof(*self)); // scrub
    autil_xalloc(self, AUTIL_XALLOC_FREE);
}

AUTIL_API void*
autil_vec_data(struct autil_vec const* self)
{
    assert(self != NULL);

    return self->data;
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

    self->data = autil_xallocn(self->data, capacity, self->elemsize);
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
        autil_fatalf("[%s] Index out of bounds", __func__);
    }

    memmove(((char*)self->data) + (idx * self->elemsize), data, self->elemsize);
}

AUTIL_API void*
autil_vec_get(struct autil_vec const* self, size_t idx)
{
    assert(self != NULL);

    if (idx >= self->count) {
        autil_fatalf("[%s] Index out of bounds", __func__);
    }

    return ((char*)self->data) + (idx * self->elemsize);
}

AUTIL_API void
autil_vec_insert(struct autil_vec* self, size_t idx, void const* data)
{
    assert(self != NULL);

    if (idx > self->count) {
        autil_fatalf("[%s] Invalid index", __func__);
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
    void* const move_src = ((char*)self->data) + (idx * self->elemsize);
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
        autil_fatalf("[%s] Invalid index", __func__);
    }

    if (oldelem != NULL) {
        memcpy(oldelem, autil_vec_get(self, idx), self->elemsize);
    }

    // [A][B][X][C][D][E]
    // [A][B][C][D][E][ ]
    size_t const move_count = (self->count - 1) - idx;
    size_t const move_size = move_count * self->elemsize;
    void* const move_dst = ((char*)self->data) + (idx * self->elemsize);
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
    assert(self != NULL);

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
map_find_(struct autil_map const* self, void const* key)
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
        void* const midkey = autil_vec_get(self->keys, (size_t)mid);
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
autil_map_lookup(struct autil_map const* self, void const* key)
{
    assert(self != NULL);

    long const location = map_find_(self, key);
    if (location < 0) {
        return NULL;
    }

    return autil_vec_get(self->vals, (size_t)location);
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

    long const location = map_find_(self, key);
    if (location < 0) {
        size_t const idx = (size_t)(-1 * location) - 1;
        autil_vec_insert(self->keys, idx, key);
        autil_vec_insert(self->vals, idx, val);
        return 0;
    }

    size_t const idx = (size_t)location;
    if (oldkey != NULL) {
        memcpy(oldkey, autil_vec_get(self->keys, idx), self->keys->elemsize);
    }
    if (oldval != NULL) {
        memcpy(oldval, autil_vec_get(self->vals, idx), self->vals->elemsize);
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

    long const location = map_find_(self, key);
    if (location < 0) {
        return 0;
    }

    size_t const idx = (size_t)location;
    autil_vec_remove(self->keys, idx, oldkey);
    autil_vec_remove(self->vals, idx, oldval);
    return 1;
}

#endif // AUTIL_IMPLEMENTATION
