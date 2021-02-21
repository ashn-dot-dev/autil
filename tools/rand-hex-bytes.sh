#!/bin/sh

set -e

usage() {
    echo "Usage: $(basename $0) N"
    echo "Generate N bytes of random data"
}

if [ $# != '1' ]; then
    usage
    exit 1
fi
N="${1}"

# TODO: POSIX does not require the device file /dev/urandom to exist[1]. Find a
# POSIX compliant solutions for producing random bytes.
# [1]: https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap10.html
DIGITS="$(\
    cat /dev/urandom | \
    od -N ${N} -t x1 | \
    awk -F ' ' '{for (i=2; i<=NF; i++){printf $i;}}')"
printf "${DIGITS}\n"
