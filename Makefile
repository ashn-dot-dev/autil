.POSIX:
.SUFFIXES:
.PHONY: \
	all \
	test \
	perf \
	examples \
	examples-web \
	format \
	clean
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
	-s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["jpg", "png"]' \
	-s USE_SDL_MIXER=2

TESTS = \
	test/static.test \
	test/include-multiple-times.test \
	test/custom-allocator.test \
	test/autil_array_count.test \
	test/autil_str_literal_count.test \
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
	test/autil_xalloc_prepend.test \
	test/autil_xalloc_append.test \
	test/autil_file_read.test \
	test/autil_file_write.test \
	test/autil_stream_read_line.test \
	test/autil_cstr_new.test \
	test/autil_cstr_new_cstr.test \
	test/autil_cstr_starts_with.test \
	test/autil_cstr_ends_with.test \
	test/autil_vstr_cmp.test \
	test/autil_vstr_starts_with.test \
	test/autil_vstr_ends_with.test \
	test/autil_vstr.example.test \
	test/autil_sbuf.example.test \
	test/autil_bitarr_count.test \
	test/autil_bitarr_set.test \
	test/autil_bitarr_get.test \
	test/autil_bitarr_assign.test \
	test/autil_bitarr_shiftl.test \
	test/autil_bitarr_shiftr.test \
	test/autil_bitarr_compl.test \
	test/autil_bitarr_and.test \
	test/autil_bitarr_or.test \
	test/autil_bitarr_xor.test \
	test/autil_bigint_new_cstr.test \
	test/autil_bigint_cmp.test \
	test/autil_bigint_to_new_cstr.test \
	test/autil_bigint_magnitude_shiftl.test \
	test/autil_bigint_magnitude_shiftr.test \
	test/autil_bigint_magnitude_bit_count.test \
	test/autil_bigint_magnitude_bit_get.test \
	test/autil_bigint_magnitude_bit_set.test \
	test/autil_bigint_add.test \
	test/autil_bigint_sub.test \
	test/autil_bigint_mul.test \
	test/autil_bigint_divrem.test \
	test/autil_string_new.test \
	test/autil_string_new_cstr.test \
	test/autil_string_new_fmt.test \
	test/autil_string_cmp.test \
	test/autil_string_resize.test \
	test/autil_string_ref.test \
	test/autil_string_ref_const.test \
	test/autil_string_insert.test \
	test/autil_string_remove.test \
	test/autil_string_append.test \
	test/autil_string_append_cstr.test \
	test/autil_string_append_fmt.test \
	test/autil_string_trim.test \
	test/autil_string_split_to_vec.test \
	test/autil_string_split_to_vec_on.test \
	test/autil_sipool.example.test \
	test/autil_vec_reserve.test \
	test/autil_vec_resize.test \
	test/autil_vec_insert.test \
	test/autil_vec_remove.test \
	test/autil_vec_set.test \
	test/autil_vec_ref.test \
	test/autil_vec_next.test \
	test/autil_vec.zero_size_elements.test \
	test/autil_map_insert.test \
	test/autil_map_remove.test \
	test/autil_map_lookup.test \
	test/autil_map.example.test \
	test/autil_map.zero_size_elements.test \
	test/atest.example.test

PERFS = \
	perf/bigint-add-4096x4096-bits.perf \
	perf/bigint-mul-256x256-bits.perf \
	perf/bigint-mul-512x512-bits.perf \
	perf/bigint-mul-1024x1024-bits.perf \
	perf/bigint-mul-2048x2048-bits.perf \
	perf/bigint-mul-4096x4096-bits.perf \
	perf/bigint-divrem-256x128-bits.perf \
	perf/bigint-divrem-512x256-bits.perf \
	perf/bigint-divrem-1024x512-bits.perf \
	perf/bigint-divrem-2048x1024-bits.perf \
	perf/bigint-divrem-4096x2048-bits.perf \
	perf/bigint-divrem-4096x64-bits.perf

EXAMPLES = \
	examples/bigint-calculator \
	examples/bigint-fib \
	examples/dvd \
	examples/life \
	examples/shapes
EXAMPLES_WEB = \
	examples/dvd.html \
	examples/life.html \
	examples/shapes.html

all: test perf examples examples-web

test: $(TESTS)
	(cd test/ && sh test-all.sh)

perf: $(PERFS)

examples: $(EXAMPLES)
examples/bigint-calculator: examples/bigint-calculator.c
	$(CC) -o $@ $(CFLAGS) examples/bigint-calculator.c
examples/bigint-fib: examples/bigint-fib.c
	$(CC) -o $@ $(CFLAGS) examples/bigint-fib.c
examples/dvd: examples/dvd.c
	$(CC) -o $@ $(CFLAGS) examples/dvd.c \
		-lm $$(sdl2-config --libs) -lSDL2_image -lSDL2_mixer
examples/life: examples/life.c
	$(CC) -o $@ $(CFLAGS) examples/life.c \
		-lm $$(sdl2-config --libs) -lSDL2_image -lSDL2_mixer
examples/shapes: examples/shapes.c
	$(CC) -o $@ $(CFLAGS) examples/shapes.c \
		-lm $$(sdl2-config --libs) -lSDL2_image -lSDL2_mixer

examples-web: $(EXAMPLES_WEB)
examples/dvd.html: examples/dvd.c
	emcc -o $@ $(CFLAGS) $(EMFLAGS) examples/dvd.c \
		--embed-file examples/dvd-assets@dvd-assets/
examples/life.html: examples/life.c
	emcc -o $@ $(CFLAGS) $(EMFLAGS) examples/life.c
examples/shapes.html: examples/shapes.c
	emcc -o $@ $(CFLAGS) $(EMFLAGS) examples/shapes.c

format:
	clang-format -i \
		*.h \
		$$(find test/ -type f -name '*.h') \
		$$(find test/ -type f -name '*.c') \
		$$(find perf/ -type f -name '*.h') \
		$$(find perf/ -type f -name '*.c') \
		$$(find examples/ -type f -name '*.h') \
		$$(find examples/ -type f -name '*.c')

clean:
	rm -f $$(find . -type f -name '*.test')
	rm -f $$(find . -type f -name '*.perf')
	rm -f $(EXAMPLES)
	rm -f $(EXAMPLES_WEB)

.SUFFIXES: .c .test .perf
.c.test:
	$(CC) -o $@ $(CFLAGS) $<
.c.perf:
	$(CC) -o $@ $(CFLAGS) $<
