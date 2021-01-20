#!/bin/sh

set -e

usage() {
    echo "Usage: $0 VERSION"
    echo "Prepare a release of autil with the provided version number."
    echo "VERSION should be in the form MAJOR.MINOR.PATCH."
    printf '\n'
    echo "Full release process:"
    echo "  \$ sh $0 \${VERSION}"
    echo "  \$ git add --all"
    echo "  \$ git commit --message='Release v\${VERSION}'"
    echo "  \$ git push origin master"
    echo "  \$ git tag v\${VERSION}"
    echo "  \$ git push origin v\${VERSION}"
}

if [ $# != '1' ]; then
    usage
    exit 1
fi

VERSION="$1"
DATE="$(date +"%Y-%m-%d")" # ISO-8601

for file in *.h; do
    cat "${file}" | \
        sed "s/ - v[0-9]*\.[0-9]*\.[0-9]*$/ - v${VERSION}/" >"${file}.tmp"
    mv "${file}.tmp" "${file}"
done

for file in CHANGELOG*.txt; do
    cat "${file}" | \
        sed "s/^Unreleased/v${VERSION} - ${DATE}/" >"${file}.tmp"
    mv "${file}.tmp" "${file}"
done
