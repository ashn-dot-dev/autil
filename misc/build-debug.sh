#!/bin/sh
set -e

usage() {
    cat <<EOF
Usage: $(basename $0) [OPTION]... [TARGETS]
Run the full build/test cycle in debug mode with appropriate flags.
Defaults to CC=clang.

Examples:
  $(basename $0)
  $(basename $0) check
  CC=tcc $(basename $0) check
  CC=gcc $(basename $0) check

Options:
  -h --help     Print this help message and exit.
EOF
}

while test "$#" -gt 0; do
case "$1" in
    -h|--help)
        usage
        exit 0
        ;;
    *)
        break
        ;;
esac
done

TARGETS="$@"
if [ -z "${TARGETS}" ]; then TARGETS="check"; fi
if [ -z "${CC}" ]; then CC=clang; fi

CFLAGS='$(C99_DBG)'
if [ "${CC}" = 'clang' ]; then
    CFLAGS='$(GNU_DBG) $(SANITIZE) -ferror-limit=1'
elif [ "${CC}" = 'gcc' ]; then
    CFLAGS='$(GNU_DBG) $(SANITIZE) -fmax-errors=1'
fi

set -x
make -j $(nproc) ${TARGETS} CC="${CC}" CFLAGS="${CFLAGS}"
