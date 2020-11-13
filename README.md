AUTIL - ASHN'S UTILITY LIBRARY(S)
=================================

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
Native applications are run as you would expect with no real surprises.
For example applications that load assets relative to the working directory, you
should run the application from within the `example/` directory.
Note that some native applications built using `aengn.h` may provide additional
command line options not seen when the binary is run by selecting it from within
a file browser.

Web applications built with separate HTML, JS, and WASM files will likely be
blocked by your browser if you just open the `.html` page.
This limitation can be avoided by serving the page with:
```sh
$ python3 -m http.server 8080
```
and navigating to `http://localhost:8080/path/to/the/example.html` within your
browser.
Currently each web application and its assets are bundled into a single `.html`
file to circumvent this issue.


## License
All content in this repository is licensed under the Zero-Clause BSD license.

See LICENSE for more information.
