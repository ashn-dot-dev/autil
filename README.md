AUTIL - ASHN'S UTILITIES
========================

Single header files containing functions and data structures for rapid
application development in C99 (or later).

See individual header files for usage instructions.

## Building and Running Unit/Integration Tests
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


## Building and Running Examples
### Dependencies
Applications built using `aengn.h` link against `SDL2` and `SDL2_image` for
native builds and compile with Emscripten's `emcc` for web builds.
SDL2 and its extension libraries are provided by most package managers, and can
be installed on Debian-based distros with:

```sh
$ apt-get install libsdl2-dev
$ apt-get install libsdl2-image-dev
```

The Emscripten toolchain can be downloaded and activated with:

```sh
$ git clone https://github.com/emscripten-core/emsdk.git
$ cd emsdk/
$ ./emsdk install latest  # Download and install latest SDK (do this once)
$ ./emsdk activate latest # Make the latest SDK active (do this once)
$ source ./emsdk_env.sh   # Setup envrionment (source this every time)
```

*NOTE*: Currently `aengn.h` and aengn example programs are known to compile on
Debian stable using `CC=gcc` or `CC=clang`, but fail to compile with
`CC=musl-gcc` and `CC=tcc`.


### Building
```sh
$ make examples CFLAGS='$(C99_DBG)'  # Debug   (c99)
$ make examples                      # Release (c99)

$ make examples CC=clang CFLAGS='$(GNU_DBG)'  # Debug   (gcc/clang)
$ make examples CC=clang CFLAGS='$(GNU_REL)'  # Release (gcc/clang)

$ make examples-web CFLAGS='$(GNU_DBG)'  # Debug   (emscripten)
$ make examples-web CFLAGS='$(GNU_REL)'  # Release (emscripten)
```

### Running
To run an example with the name `example-name`:
```sh
$ (cd examples/ && ./example-name)
```
which will set the working directory to `example/` for applications that load
assets via relative paths.


## License
All content in this repository is licensed under the Zero-Clause BSD license.

See LICENSE for more information.
