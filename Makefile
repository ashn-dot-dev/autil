.POSIX:
.SUFFIXES:
.PHONY: \
	all \
	check \
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
	tests/static.test \
	tests/include-multiple-times.test \
	tests/custom-allocator.test \
	tests/autil_array_count.test \
	tests/autil_str_literal_count.test \
	tests/autil_fmt_count.test \
	tests/autil_isalnum.test \
	tests/autil_isalpha.test \
	tests/autil_isblank.test \
	tests/autil_iscntrl.test \
	tests/autil_isdigit.test \
	tests/autil_isgraph.test \
	tests/autil_islower.test \
	tests/autil_isprint.test \
	tests/autil_ispunct.test \
	tests/autil_isspace.test \
	tests/autil_isupper.test \
	tests/autil_isxdigit.test \
	tests/autil_tolower.test \
	tests/autil_toupper.test \
	tests/autil_infof.test \
	tests/autil_errorf.test \
	tests/autil_fatalf.test \
	tests/autil_xalloc.test \
	tests/autil_xallocn.test \
	tests/autil_xalloc_prepend.test \
	tests/autil_xalloc_append.test \
	tests/autil_file_read.test \
	tests/autil_file_write.test \
	tests/autil_stream_read_line.test \
	tests/autil_cstr_new.test \
	tests/autil_cstr_new_cstr.test \
	tests/autil_cstr_new_fmt.test \
	tests/autil_cstr_starts_with.test \
	tests/autil_cstr_ends_with.test \
	tests/autil_vstr_cmp.test \
	tests/autil_vstr_starts_with.test \
	tests/autil_vstr_ends_with.test \
	tests/autil_vstr.example.test \
	tests/autil_sbuf.example.test \
	tests/autil_bitarr_count.test \
	tests/autil_bitarr_set.test \
	tests/autil_bitarr_get.test \
	tests/autil_bitarr_assign.test \
	tests/autil_bitarr_shiftl.test \
	tests/autil_bitarr_shiftr.test \
	tests/autil_bitarr_compl.test \
	tests/autil_bitarr_and.test \
	tests/autil_bitarr_or.test \
	tests/autil_bitarr_xor.test \
	tests/autil_bigint_new_cstr.test \
	tests/autil_bigint_cmp.test \
	tests/autil_bigint_to_new_cstr.test \
	tests/autil_bigint_magnitude_shiftl.test \
	tests/autil_bigint_magnitude_shiftr.test \
	tests/autil_bigint_magnitude_bit_count.test \
	tests/autil_bigint_magnitude_bit_get.test \
	tests/autil_bigint_magnitude_bit_set.test \
	tests/autil_bigint_add.test \
	tests/autil_bigint_sub.test \
	tests/autil_bigint_mul.test \
	tests/autil_bigint_divrem.test \
	tests/autil_string_new.test \
	tests/autil_string_new_cstr.test \
	tests/autil_string_new_fmt.test \
	tests/autil_string_cmp.test \
	tests/autil_string_resize.test \
	tests/autil_string_ref.test \
	tests/autil_string_ref_const.test \
	tests/autil_string_insert.test \
	tests/autil_string_remove.test \
	tests/autil_string_append.test \
	tests/autil_string_append_cstr.test \
	tests/autil_string_append_fmt.test \
	tests/autil_string_trim.test \
	tests/autil_string_split.test \
	tests/autil_vec_reserve.test \
	tests/autil_vec_resize.test \
	tests/autil_vec_insert.test \
	tests/autil_vec_remove.test \
	tests/autil_vec_set.test \
	tests/autil_vec_ref.test \
	tests/autil_vec_next.test \
	tests/autil_vec.zero_size_elements.test \
	tests/autil_map_insert.test \
	tests/autil_map_remove.test \
	tests/autil_map_lookup.test \
	tests/autil_map.example.test \
	tests/autil_map.zero_size_elements.test \
	tests/autil_freezer.example.test \
	tests/atest.example.test

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

all: check perf examples examples-web

check: $(TESTS)
	(cd tests/ && sh test-all.sh)

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
		$$(find tests/ -type f -name '*.h') \
		$$(find tests/ -type f -name '*.c') \
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
