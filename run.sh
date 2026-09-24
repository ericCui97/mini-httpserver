#!/bin/sh
set -eu
cd "$(dirname "$0")"
cmake -S . -B build-local
cmake --build build-local -j 4
cd build-local
# Generate the missing download fixture locally; do not commit a 100 MiB file.
if [ ! -f 100M ]; then
    dd if=/dev/zero of=100M bs=1048576 count=100 2>/dev/null
fi
exec ./epollserver
