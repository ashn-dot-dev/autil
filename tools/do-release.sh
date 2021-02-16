#!/bin/sh

set -e

usage() {
    echo "Usage: $(basename $0) VERSION"
    echo "Prepare a release of autil with the provided version number."
    echo "VERSION should be in the form v{MAJOR}.{MINOR}.{PATCH}:"
    echo "Example:"
    echo "  \$ sh $(basename $0) v3.14.1"
    printf '\n'
    echo "Full release process:"
    echo "  \$ sh $0 \${VERSION}"
    echo "  \$ git add --all"
    echo "  \$ git commit --message='Release \${VERSION}'"
    echo "  \$ git push \${REMOTE(s)} master"
    echo "  \$ git tag \${VERSION}"
    echo "  \$ git push origin \${VERSION}"
}

if [ $# != '1' ]; then
    usage
    exit 1
fi

VERSION="$1"
DATE="$(date +"%Y-%m-%d")" # ISO-8601

for file in *.h; do
    cat "${file}" | \
        sed "s/ - v[0-9]*\.[0-9]*\.[0-9]*$/ - ${VERSION}/" >"${file}.tmp"
    mv "${file}.tmp" "${file}"
done

for file in CHANGELOG*.txt; do
    cat "${file}" | \
        sed "s/^Unreleased/${VERSION} - ${DATE}/" >"${file}.tmp"
    mv "${file}.tmp" "${file}"
done
