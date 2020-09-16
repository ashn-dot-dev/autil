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

#define AUTIL_VERSION_MAJOR 0
#define AUTIL_VERSION_MINOR 1
#define AUTIL_VERSION_PATCH 0

#ifndef AUTIL_API
#    define AUTIL_API extern
#endif

#include <stddef.h>

// Should return an int less than, equal to, or greater than zero if lhs is
// semantically less than, equal to, or greater than rhs, respectively.
typedef int (*vpcmp_fn)(void const* lhs, void const* rhs);
// Implementations of vpcmp_fn for builtin types.
AUTIL_API int
str_vpcmp(void const* lhs, void const* rhs); // char const*
AUTIL_API int
int_vpcmp(void const* lhs, void const* rhs); // int

// Produce a pointer of type TYPE* whose contents is the scalar rvalue val.
// This pointer has automatic storage duration associated with the enclosing
// block.
//
// Example:
//  int* pint = LOCAL_PTR(int, 42);
//  char const** pstr = LOCAL_PTR(char const*, "FOO");
//  printf("%d %s\n", *pint, *pstr); // 42 FOO
#define LOCAL_PTR(TYPE, /*val*/...) (&((TYPE){__VA_ARGS__}))

// Dereference ptr as if it were of type TYPE*.
//
// Example:
//  void* ptr = some_func();
//  int val = DEREF_PTR(int, ptr); // Dereference ptr as if it were an int*.
#define DEREF_PTR(TYPE, /*ptr*/...) (*(TYPE*)(__VA_ARGS__))

// Number of elements in an array.
#define ARRAY_COUNT(array_) (sizeof(array_) / sizeof((array_)[0]))
// Number of characters in a cstring literal, excluding the null-terminator.
#define CSTR_COUNT(cstr_) (ARRAY_COUNT(cstr_) - 1)

// Write a formatted error message to stderr.
// A newline is automatically appended to the end of the formatted message.
AUTIL_API void
infof(char const* fmt, ...);
// Write a formatted error message to stderr.
// A newline is automatically appended to the end of the formatted message.
AUTIL_API void
errorf(char const* fmt, ...);
// Write a formatted error message to stderr and exit with EXIT_FAILURE status.
// A newline is automatically appended to the end of the formatted message.
AUTIL_API void
fatalf(char const* fmt, ...);

// General purpose allocator functions with out-of-memory error checking.
// The behavior of xalloc and xallocn is similar to libc realloc and *BSD
// reallocarray with the following exceptions:
// (1) On allocation failure an error message will be printed followed by
//     program termination with EXIT_FAILURE status.
// (2) The call xalloc(ptr, 0) is guaranteed to free the memory backing ptr.
//     A pointer returned by xalloc may be freed with xalloc(ptr, 0) or the
//     equivalent xalloc(ptr, XALLOC_FREE). The calls xallocn(ptr, x, 0) and
//     xallocn(ptr, 0, y) are equivalent to xalloc(ptr, 0).
// The macro XALLOC_FREE may be used in place of the constant zero to indicate
// that a call xalloc(ptr, XALLOC_FREE) is intended as a free operation.
AUTIL_API void*
xalloc(void* ptr, size_t size);
//
AUTIL_API void*
xallocn(void* ptr, size_t nmemb, size_t size);
//
#define XALLOC_FREE ((size_t)0)

// Read the full contents of the file sepcified by path.
// Returns zero on success, in which case a pointer to the xalloc-allocated
// buffer and that buffer's size are stored in *buf and *buf_size.
AUTIL_API int
file_read(char const* path, void** buf, size_t* buf_size);

// Write the contents of a buffer into the file specified by path.
// The file specified by path is created if it does not exist.
// Returns zero on success.
// On failure, the contents of the file specified by path is undefined.
AUTIL_API int
file_write(char const* path, void const* buf, size_t buf_size);

////////////////////////////////////////////////////////////////////////////////
//////// VEC ///////////////////////////////////////////////////////////////////

// General purpose generic resizeable array.
// A vec conceptually consists of the following components:
// (1) data - An array containing the elements of the vec.
// (2) count - The number of elements in the vec.
// (3) capacity - The total number of elements allocated in the data array.
//     This value is always greater than or equal to the count of the vec.
struct vec;

// Allocate and initialize a vec holding elements of size elemsize.
AUTIL_API struct vec*
vec_new(size_t elemsize);
// Deinitialize and free the vec.
AUTIL_API void
vec_del(struct vec* self);

// Pointer to the start of the underlying array of the vec.
// May return NULL when the count of the vec is zero.
AUTIL_API void*
vec_data(struct vec const* self);
// The number of elements in the vec.
AUTIL_API size_t
vec_count(struct vec const* self);
// The number of elements allocated in the vec.
AUTIL_API size_t
vec_capacity(struct vec const* self);

// Update the minimum capacity of the vec.
// The count of the vec remains unchanged.
AUTIL_API void
vec_reserve(struct vec* self, size_t capacity);
// Update the count of the vec.
// If count is greater than the current count of the vec then additional
// elements are initialized with garbage data.
AUTIL_API void
vec_resize(struct vec* self, size_t count);

// Set the value of the vec at position idx to the value at data.
// Fatally exits after printing an error message if idx is out of bounds.
AUTIL_API void
vec_set(struct vec* self, size_t idx, void const* data);
// Get a pointer to the value of the vec at position idx.
// Fatally exits after printing an error message if idx is out of bounds.
AUTIL_API void*
vec_get(struct vec const* self, size_t idx);

// Insert a copy of the value at data into the vec at position idx.
// Elements with position greater than or equal to idx are moved back one place.
// Fatally exits after printing an error message if idx is greater than the
// count of the vec.
AUTIL_API void
vec_insert(struct vec* self, size_t idx, void const* data);
// Remove the element at position idx from the vec.
// Elements with position greater than idx are moved forward one place.
// If oldelem is not NULL then the removed element will be copied into oldelem.
// Fatally exits after printing an error message if idx is out of bounds.
AUTIL_API void
vec_remove(struct vec* self, size_t idx, void* oldelem);

////////////////////////////////////////////////////////////////////////////////
//////// MAP ///////////////////////////////////////////////////////////////////

// General purpose generic ordered map.
// Maps keys of some key-type to values of some value-type.
struct map;

// Allocate and initialize a map.
// The map will hold keys of size keysize.
// The map will hold values of size valsize.
// Keys-value pairs in the map are sorted by key using keycmp.
AUTIL_API struct map*
map_new(size_t keysize, size_t valsize, vpcmp_fn keycmp);
// Deinitialize and free the map.
AUTIL_API void
map_del(struct map* self);

// The number of key-value pairs in the map.
AUTIL_API size_t
map_count(struct map const* self);
// Reference to the ordered vec of keys in the map.
AUTIL_API struct vec const*
map_keys(struct map const* self);
// Reference to the ordered vec of values in the map.
AUTIL_API struct vec const*
map_vals(struct map const* self);

// Retrieve a pointer to the value associated with key.
// Returns NULL if the map has no key-value pair associated with key.
AUTIL_API void*
map_lookup(struct map const* self, void const* key);
// Insert a copy of the key-value pair at key/val into the map.
// If a key-value pair with key already exists in the map then the existing key
// and value are replaced with the provided key and value.
//
// If oldkey is not NULL then the removed key will be copied into oldkey.
// If oldval is not NULL then the removed value will be copied into oldval.
// Returns non-zero value if a key-value pair associated with key was replaced
// by the provided key and value.
AUTIL_API int
map_insert(
    struct map* self,
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
map_remove(struct map* self, void const* key, void* oldkey, void* oldval);

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
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AUTIL_API int
str_vpcmp(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    return strcmp(*(char const**)lhs, *(char const**)rhs);
}

AUTIL_API int
int_vpcmp(void const* lhs, void const* rhs)
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

AUTIL_API void
infof(char const* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fputs("info: ", stderr);
    vfprintf(stderr, fmt, args);
    fputs("\n", stderr);

    va_end(args);
}

AUTIL_API void
errorf(char const* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fputs("error: ", stderr);
    vfprintf(stderr, fmt, args);
    fputs("\n", stderr);

    va_end(args);
}

AUTIL_API void
fatalf(char const* fmt, ...)
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
xalloc(void* ptr, size_t size)
{
    if (size == 0) {
        AUTIL_FREE(ptr);
        return NULL;
    }
    if ((ptr = AUTIL_REALLOC(ptr, size)) == NULL) {
        fatalf("[%s] Out of memory", __func__);
    }
    return ptr;
}

AUTIL_API void*
xallocn(void* ptr, size_t nmemb, size_t size)
{
    size_t const sz = nmemb * size;
    if (nmemb != 0 && sz / nmemb != size) {
        fatalf("[%s] Integer overflow", __func__);
    }
    return xalloc(ptr, sz);
}

AUTIL_API int
file_read(char const* path, void** buf, size_t* buf_size)
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
        bf = xalloc(bf, sz + 1);
        bf[sz++] = (unsigned char)c;
    }
    if (ferror(stream)) {
        xalloc(bf, XALLOC_FREE);
        return -1;
    }

    (void)fclose(stream);
    *buf = bf;
    *buf_size = sz;
    return 0;
}

AUTIL_API int
file_write(char const* path, void const* buf, size_t buf_size)
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

struct vec
{
    void* data;
    size_t count;
    size_t capacity;
    size_t elemsize;
};

AUTIL_API struct vec*
vec_new(size_t elemsize)
{
    struct vec* const self = xalloc(NULL, sizeof(struct vec));
    self->data = NULL;
    self->count = 0;
    self->capacity = 0;
    self->elemsize = elemsize;
    return self;
}

AUTIL_API void
vec_del(struct vec* self)
{
    assert(self != NULL);

    xalloc(self->data, XALLOC_FREE);
    memset(self, 0x00, sizeof(*self)); // scrub
    xalloc(self, XALLOC_FREE);
}

AUTIL_API void*
vec_data(struct vec const* self)
{
    assert(self != NULL);

    return self->data;
}

AUTIL_API size_t
vec_count(struct vec const* self)
{
    assert(self != NULL);

    return self->count;
}

AUTIL_API size_t
vec_capacity(struct vec const* self)
{
    assert(self != NULL);

    return self->capacity;
}

AUTIL_API void
vec_reserve(struct vec* self, size_t capacity)
{
    assert(self != NULL);

    if (capacity <= self->capacity) {
        return;
    }

    self->data = xallocn(self->data, capacity, self->elemsize);
    self->capacity = capacity;
}

AUTIL_API void
vec_resize(struct vec* self, size_t count)
{
    assert(self != NULL);

    if (count > self->capacity) {
        vec_reserve(self, count);
    }

    self->count = count;
}

AUTIL_API void
vec_set(struct vec* self, size_t idx, void const* data)
{
    assert(self != NULL);

    if (idx >= self->count) {
        fatalf("[%s] Index out of bounds", __func__);
    }

    memmove(((char*)self->data) + (idx * self->elemsize), data, self->elemsize);
}

AUTIL_API void*
vec_get(struct vec const* self, size_t idx)
{
    assert(self != NULL);

    if (idx >= self->count) {
        fatalf("[%s] Index out of bounds", __func__);
    }

    return ((char*)self->data) + (idx * self->elemsize);
}

AUTIL_API void
vec_insert(struct vec* self, size_t idx, void const* data)
{
    assert(self != NULL);
    if (idx > self->count) {
        fatalf("[%s] Invalid index", __func__);
    }

    // [A][B][C][D][E]
    // [A][B][C][D][E][ ]
    if (self->count == self->capacity) {
        static size_t const GROWTH_FACTOR = 2;
        static size_t const DEFAULT_CAPACITY = 32;
        size_t const capacity =
            self->count ? self->count * GROWTH_FACTOR : DEFAULT_CAPACITY;
        vec_reserve(self, capacity);
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
    vec_set(self, idx, data);
}

AUTIL_API void
vec_remove(struct vec* self, size_t idx, void* oldelem)
{
    assert(self != NULL);
    if (idx >= self->count) {
        fatalf("[%s] Invalid index", __func__);
    }

    if (oldelem != NULL) {
        memcpy(oldelem, vec_get(self, idx), self->elemsize);
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

struct map
{
    struct vec* keys;
    struct vec* vals;
    vpcmp_fn keycmp;
};

AUTIL_API struct map*
map_new(size_t keysize, size_t valsize, vpcmp_fn keycmp)
{
    struct map* const self = xalloc(NULL, sizeof(struct map));
    self->keys = vec_new(keysize);
    self->vals = vec_new(valsize);
    self->keycmp = keycmp;
    return self;
}

AUTIL_API void
map_del(struct map* self)
{
    assert(self != NULL);

    vec_del(self->keys);
    vec_del(self->vals);
    memset(self, 0x00, sizeof(*self)); // scrub
    xalloc(self, XALLOC_FREE);
}

AUTIL_API size_t
map_count(struct map const* self)
{
    assert(self != NULL);

    return vec_count(self->keys);
}

AUTIL_API struct vec const*
map_keys(struct map const* self)
{
    assert(self != NULL);

    return self->keys;
}

AUTIL_API struct vec const*
map_vals(struct map const* self)
{
    assert(self != NULL);

    return self->vals;
}

// Returns the (positive) index of key if it exists in self.
// Returns a negative number that, if negated and subtracting one from, would be
// the index of key if inserted (I.E -1 means insert at 0 and -42 means insert
// at 41).
static long
map_find_(struct map const* self, void const* key)
{
    assert(self != NULL);
    assert(map_count(self) <= LONG_MAX);

    if (map_count(self) == 0) {
        return -1;
    }

    long bot = 0;
    long top = (long)map_count(self) - 1;
    long mid;
    while (bot <= top) {
        mid = (bot + top) / 2;
        void* const midkey = vec_get(self->keys, (size_t)mid);
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
        fatalf("Unreachable!");
    }

    return (-1 * bot) - 1;
}

AUTIL_API void*
map_lookup(struct map const* self, void const* key)
{
    assert(self != NULL);

    long const location = map_find_(self, key);
    if (location < 0) {
        return NULL;
    }

    return vec_get(self->vals, (size_t)location);
}

AUTIL_API int
map_insert(
    struct map* self,
    void const* key,
    void const* val,
    void* oldkey,
    void* oldval)
{
    assert(self != NULL);

    long const location = map_find_(self, key);
    if (location < 0) {
        size_t const idx = (size_t)(-1 * location) - 1;
        vec_insert(self->keys, idx, key);
        vec_insert(self->vals, idx, val);
        return 0;
    }

    size_t const idx = (size_t)location;
    if (oldkey != NULL) {
        memcpy(oldkey, vec_get(self->keys, idx), self->keys->elemsize);
    }
    if (oldval != NULL) {
        memcpy(oldval, vec_get(self->vals, idx), self->vals->elemsize);
    }
    vec_set(self->keys, idx, key);
    vec_set(self->vals, idx, val);
    return 1;
}

AUTIL_API int
map_remove(struct map* self, void const* key, void* oldkey, void* oldval)
{
    assert(self != NULL);

    long const location = map_find_(self, key);
    if (location < 0) {
        return 0;
    }

    size_t const idx = (size_t)location;
    vec_remove(self->keys, idx, oldkey);
    vec_remove(self->vals, idx, oldval);
    return 1;
}

#endif // AUTIL_IMPLEMENTATION
