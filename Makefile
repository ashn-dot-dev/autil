.POSIX:
.SUFFIXES:
.PHONY: \
	all \
	test \
	examples \
	examples-web \
	clean \
	format
.SILENT: clean

C99_DBG = -O0 -g
C99_REL = -DNDEBUG

GNU_BASE = \
	-std=c99 -pedantic-errors \
	-Wall -Wextra \
	-Werror=conversion \
	-Werror=double-promotion \
	-Werror=float-equal \
	-Werror=implicit-function-declaration \
	-Werror=incompatible-pointer-types \
	-Werror=vla
GNU_DBG = $(GNU_BASE) -O0 -g
GNU_REL = $(GNU_BASE) -Os -DNDEBUG
SANITIZE = -fsanitize=address -fsanitize=leak -fsanitize=undefined

CC = c99
CFLAGS = $(C99_REL)

EMFLAGS = \
	--shell-file examples/emscripten-shell.html \
	-s SINGLE_FILE=1 \
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'

TESTS = \
	test/static.test \
	test/custom-allocator.test \
	test/autil_array_count.test \
	test/autil_cstr_count.test \
	test/autil_fmt_count.test \
	test/autil_isalnum.test \
	test/autil_isalpha.test \
	test/autil_isblank.test \
	test/autil_iscntrl.test \
	test/autil_isdigit.test \
	test/autil_isgraph.test \
	test/autil_islower.test \
	test/autil_isprint.test \
	test/autil_ispunct.test \
	test/autil_isspace.test \
	test/autil_isupper.test \
	test/autil_isxdigit.test \
	test/autil_tolower.test \
	test/autil_toupper.test \
	test/autil_infof.test \
	test/autil_errorf.test \
	test/autil_fatalf.test \
	test/autil_xalloc.test \
	test/autil_xallocn.test \
	test/autil_file_read.test \
	test/autil_file_write.test \
	test/autil_vec_reserve.test \
	test/autil_vec_resize.test \
	test/autil_vec_insert.test \
	test/autil_vec_remove.test \
	test/autil_vec_set.test \
	test/autil_vec_get.test \
	test/autil_map_insert.test \
	test/autil_map_remove.test \
	test/autil_map_lookup.test \
	test/autil_map.example.test

EXAMPLES = \
	examples/dvd \
	examples/life \
	examples/shapes
EXAMPLES_WEB = \
	examples/dvd.html \
	examples/life.html \
	examples/shapes.html

all: test examples examples-web

test: $(TESTS)
	(cd test/ && sh test-all.sh)

examples: $(EXAMPLES)
examples/dvd: examples/dvd.c
	$(CC) -o $@ $< $(CFLAGS) -lm $$(sdl2-config --libs) -lSDL2_image
examples/life: examples/life.c
	$(CC) -o $@ $< $(CFLAGS) -lm $$(sdl2-config --libs) -lSDL2_image
examples/shapes: examples/shapes.c
	$(CC) -o $@ $< $(CFLAGS) -lm $$(sdl2-config --libs) -lSDL2_image

examples-web: $(EXAMPLES_WEB)
examples/dvd.html: examples/dvd.c
	emcc -o $@ $< $(CFLAGS) $(EMFLAGS) \
		--embed-file examples/dvd-assets@dvd-assets/
examples/life.html: examples/life.c
	emcc -o $@ $< $(CFLAGS) $(EMFLAGS)
examples/shapes.html: examples/shapes.c
	emcc -o $@ $< $(CFLAGS) $(EMFLAGS)

clean:
	rm -f $(TESTS)
	rm -f $(EXAMPLES)
	rm -f $(EXAMPLES_WEB)

format:
	clang-format -i \
		*.h \
		$$(find test/ -type f -name '*.h') \
		$$(find test/ -type f -name '*.c') \
		$$(find examples/ -type f -name '*.h') \
		$$(find examples/ -type f -name '*.c')

.SUFFIXES: .c .test
.c.test:
	$(CC) -o $@ $(CFLAGS) $<
