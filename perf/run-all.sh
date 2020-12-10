#!/bin/sh

for p in $(find -type f -name '*.perf'); do "${p}"; done
