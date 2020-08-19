.POSIX:
.SUFFIXES:
.PHONY: build run clean

TESTS = \
	static.test \
	custom-allocator.test \
	infof.test \
	errorf.test \
	fatalf.test \
	xalloc.test \
	xallocn.test \
	file_read.test \
	file_write.test \
	vec_reserve.test \
	vec_resize.test \
	vec_insert.test \
	vec_remove.test \
	vec_set.test \
	vec_get.test \
	map_insert.test \
	map_remove.test \
	map_lookup.test \
	map.example.test

build: $(TESTS)

run: build
	sh test-all.sh

clean:
	rm -f *.test

.SUFFIXES: .c .test
.c.test:
	$(CC) -o $@ $(CFLAGS) $<
