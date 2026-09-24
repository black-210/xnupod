#!/bin/sh

echo "=== xnu++ architecture probe ==="
echo

check_arch() {
    arch="$1"

    if grep -RqsE \
        "(CONFIG_${arch}|ARCH_${arch}|__${arch}__|${arch})" \
        osfmk bsd iokit pexpert config 2>/dev/null
    then
        echo "[FOUND] $arch"
    else
        echo "[NO]    $arch"
    fi
}

check_arch x86_64
check_arch arm64
check_arch arm
check_arch riscv64
check_arch powerpc
check_arch i386

echo
echo "Host:"
uname -m
