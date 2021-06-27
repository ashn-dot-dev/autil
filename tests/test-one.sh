#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo 'error: Too many arguments provided'
    exit 1
fi
TESTNAME="${1}"

TESTPROG="${TESTNAME}.test"      # Compiled test program
EXPECTED="${TESTNAME}.expected"  # Expected output of $TESTPROG
TEARDOWN="${TESTNAME}.teardown"  # Script run after the test

if [ -z "${TESTPROG}" ]; then
    echo "error: Missing '${TESTPROG}'"
    exit 1
fi
if [ ! -f "${EXPECTED}" ]; then
    EXPECTED=/dev/null
fi


echo "[= RUN ${TESTNAME} =]"
RESULT='PASS'
if ! "./${TESTPROG}" 2>&1 | diff - "${EXPECTED}"; then
    RESULT='FAIL'
fi
if [ -f "${TEARDOWN}" ] && ! "./${TEARDOWN}"; then
    RESULT='FAIL'
fi

[ "${RESULT}" = 'PASS' ] && echo '[= PASS =]' && exit 0
[ "${RESULT}" = 'FAIL' ] && echo '[= FAIL =]' && exit 1
