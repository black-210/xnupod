#!/bin/sh

echo "=== xnu++ capability probe ==="
echo

check() {
    name="$1"
    shift

    if "$@" >/dev/null 2>&1; then
        printf "[YES] %s\n" "$name"
    else
        printf "[NO]  %s\n" "$name"
    fi
}

echo "[Project]"

check "XNU source tree" test -d osfmk
check "BSD source tree" test -d bsd
check "IOKit source tree" test -d iokit
check "xnu++ headers" test -d include/xnu++
check "xnu++ tools" test -d tools/xnu++
check "distribution policies" test -d distribution

echo
echo "[xnu++ interfaces]"

check "compat.h" test -f include/xnu++/compat.h
check "security.h" test -f include/xnu++/security.h
check "device.h" test -f include/xnu++/device.h
check "bus.h" test -f include/xnu++/bus.h
check "reliability.h" test -f include/xnu++/reliability.h
check "update.h" test -f include/xnu++/update.h
check "recovery.h" test -f include/xnu++/recovery.h
check "quota.h" test -f include/xnu++/quota.h
check "isolation.h" test -f include/xnu++/isolation.h
check "diagnostics.h" test -f include/xnu++/diagnostics.h

echo
echo "[Toolchain]"

check "clang" command -v clang
check "clang++" command -v clang++
check "ld" command -v ld
check "make" command -v make
check "python3" command -v python3

echo
echo "[Architecture compilation probes]"

probe_arch() {
    arch="$1"
    probe_dir="${TMPDIR:-.}/.xnu++-probe"
    mkdir -p "$probe_dir"
    file="$probe_dir/xnu++_${arch}.c"
    obj="$probe_dir/xnu++_${arch}.o"

    printf 'int xnu_probe(void) { return 0; }\n' > "$file"

    if clang -target "$arch-unknown-none" -ffreestanding -c "$file" -o "$obj" >/dev/null 2>&1; then
        echo "[YES] $arch compilation"
        rm -f "$file" "$obj"
        return 0
    fi

    echo "[NO]  $arch compilation"
    rm -f "$file" "$obj"
    return 1
}

probe_arch x86_64
probe_arch aarch64
probe_arch arm
probe_arch riscv64

echo
echo "[Platform information]"

printf "Host OS: "
uname -s 2>/dev/null || echo unknown

printf "Host architecture: "
uname -m 2>/dev/null || echo unknown

printf "Compiler: "
clang --version 2>/dev/null | head -n 1 || echo unavailable

echo
echo "[Policy files]"

check "default profile" test -f distribution/default.profile
check "security profile" test -f distribution/security.profile
check "reliability profile" test -f distribution/reliability.profile
check "update policy" test -f distribution/update.policy
check "recovery policy" test -f distribution/recovery.policy

echo
echo "[Metadata]"

check "capabilities.json" test -f tools/xnu++/capabilities.json
check "support-matrix.json" test -f tools/xnu++/support-matrix.json
check "reliability-matrix.json" test -f tools/xnu++/reliability-matrix.json

echo
echo "=== Probe complete ==="
echo
echo "YES = detected/passed"
echo "NO  = unavailable/failed"
echo "UNKNOWN/untested hardware support must not be inferred from these results."
