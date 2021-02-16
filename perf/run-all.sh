#!/bin/sh

for p in $(find . -type f -name '*.perf' | sort); do "${p}"; done
