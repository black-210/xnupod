#!/bin/sh

set -eu

if [ ! -d "osfmk" ] || [ ! -d "bsd" ]; then
    echo "NO"
    exit 1
fi

if ! command -v x86_64-apple-darwin-clang >/dev/null 2>&1 &&
   ! command -v clang >/dev/null 2>&1; then
    echo "NO"
    exit 1
fi

if command -v clang >/dev/null 2>&1; then
    if clang -arch x86_64 -x c -c /dev/null -o /tmp/xnu_x86_test.o >/dev/null 2>&1; then
        rm -f /tmp/xnu_x86_test.o
        echo "YES"
        exit 0
    fi
fi

echo "NO"
exit 1
