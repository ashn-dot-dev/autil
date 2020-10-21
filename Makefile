.POSIX:
.SUFFIXES:
.PHONY: \
	all \
	test \
	examples \
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

all: test examples

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
test: $(TESTS)
	(cd test/ && sh test-all.sh)

clean:
	rm -f $(TESTS)
	cd examples && $(MAKE) clean

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
