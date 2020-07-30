AUTIL - ASHN'S UTILITY LIBRARY
==============================

Single header file containing functions and data structures for rapid
application development in C99 (or later) with a focus on simplicity.
This library has no additional dependencies outside of libc.

See `autil.h` for usage instructions.

## Running Unit/Integration Tests
### POSIX c99
```sh
$ make test CFLAGS='$(C99_DBG)'  # Debug

$ make test                      # Release
```

### GCC or Clang
```sh
$ make test CC=clang CFLAGS='$(GNU_DBG)'              # Debug

$ make test CC=clang CFLAGS='$(GNU_DBG) $(SANITIZE)'  # Debug with Address
                                                      # Sanitizer (glibc only)

$ make test CC=clang CFLAGS='$(GNU_REL)'              # Release
```

## License
All content in this repository is licensed under the Zero-Clause BSD license.

See LICENSE for more information.
