#!/bin/sh

TESTSRUN=0
FAILURES=0
for t in $(find -type f -name  '*.test' | sort); do
    TESTNAME=$(basename "${t}" .test)
    sh test-one.sh "${TESTNAME}"
    RESULT="$?"

    TESTSRUN=$((TESTSRUN + 1))
    if [ "${RESULT}" -ne 0 ]; then
        FAILURES=$((FAILURES + 1))
    fi
done

echo "TESTS RUN => ${TESTSRUN}"
echo "FAILURES  => ${FAILURES}"

[ "${FAILURES}" -eq 0 ] || exit 1
