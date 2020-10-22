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

EMFLAGS = \
	--shell-file examples/emscripten-shell.html \
	-s SINGLE_FILE=1 \
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'

CC = c99
CFLAGS = $(C99_REL)

TESTS = \
	test/static.test \
	test/custom-allocator.test \
	test/infof.test \
	test/errorf.test \
	test/fatalf.test \
	test/xalloc.test \
	test/xallocn.test \
	test/file_read.test \
	test/file_write.test \
	test/vec_reserve.test \
	test/vec_resize.test \
	test/vec_insert.test \
	test/vec_remove.test \
	test/vec_set.test \
	test/vec_get.test \
	test/map_insert.test \
	test/map_remove.test \
	test/map_lookup.test \
	test/map.example.test

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
	$(CC) -o $@ $< $(CFLAGS) -lm -lSDL2 -lSDL2_image
examples/life: examples/life.c
	$(CC) -o $@ $< $(CFLAGS) -lm -lSDL2 -lSDL2_image
examples/shapes: examples/shapes.c
	$(CC) -o $@ $< $(CFLAGS) -lm -lSDL2 -lSDL2_image

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
