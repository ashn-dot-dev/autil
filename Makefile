.POSIX:
.SUFFIXES:
.PHONY: test clean format
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

all: test

test:
	@echo '== TEST =='
	cd test && $(MAKE) CC='$(CC)' CFLAGS='$(CFLAGS)' run

clean:
	@echo '== CLEAN =='
	cd test && $(MAKE) clean

format:
	@echo '== FORMAT =='
	clang-format -i \
		*.h \
		test/*.h test/*.c
