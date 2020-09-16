.POSIX:
.SUFFIXES:
.PHONY: all test examples clean format
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

test:
	@echo '== TEST =='
	cd test && $(MAKE) run CC='$(CC)' CFLAGS='$(CFLAGS)'

examples:
	@echo '== EXAMPLES =='
	cd examples && $(MAKE) build CC='$(CC)' CFLAGS='$(CFLAGS)'

clean:
	@echo '== CLEAN =='
	cd test && $(MAKE) clean
	cd examples && $(MAKE) clean

format:
	@echo '== FORMAT =='
	clang-format -i \
		*.h \
		$$(find test/ -type f -name '*.h') \
		$$(find test/ -type f -name '*.c') \
		$$(find examples/ -type f -name '*.h') \
		$$(find examples/ -type f -name '*.c')
