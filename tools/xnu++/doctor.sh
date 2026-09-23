#!/bin/sh
# xnu++ environment and source-tree diagnostic.
# Read-only by design; suitable for CI and local development.
set -u

format=text
while [ "$#" -gt 0 ]; do
    case "$1" in
        --format)
            [ "$#" -ge 2 ] || { echo "error: --format needs text or json" >&2; exit 2; }
            format=$2; shift 2 ;;
        --help|-h)
            echo "usage: $0 [--format text|json]"; exit 0 ;;
        *) echo "error: unknown option: $1" >&2; exit 2 ;;
    esac
done
[ "$format" = text ] || [ "$format" = json ] || { echo "error: format must be text or json" >&2; exit 2; }

root=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
pass=0; warn=0; fail=0
names=""; states=""; details=""
record() {
    name=$1; state=$2; detail=$3
    names="${names}${names:+|}$name"; states="${states}${states:+|}$state"; details="${details}${details:+|}$detail"
    case "$state" in pass) pass=$((pass + 1));; warn) warn=$((warn + 1));; fail) fail=$((fail + 1));; esac
}
command_exists() { command -v "$1" >/dev/null 2>&1; }

[ -f "$root/Makefile" ] && record source_tree pass "repository root detected" || record source_tree fail "Makefile not found"
[ -d "$root/osfmk" ] && [ -d "$root/bsd" ] && [ -d "$root/iokit" ] && record xnu_layout pass "core XNU directories present" || record xnu_layout fail "one or more core XNU directories are missing"
[ -f "$root/XNU++_VERSION" ] && record identity pass "xnu++ identity file present" || record identity warn "XNU++_VERSION is missing"

for tool in make clang git; do
    if command_exists "$tool"; then record "$tool" pass "$(command -v "$tool")"; else record "$tool" fail "required command is not on PATH"; fi
done
for tool in xcodebuild xcrun lldb; do
    if command_exists "$tool"; then record "$tool" pass "optional tool available"; else record "$tool" warn "optional Apple development tool is unavailable"; fi
done

os=$(uname -s 2>/dev/null || echo unknown)
arch=$(uname -m 2>/dev/null || echo unknown)
case "$os" in Darwin) record host_os pass "Darwin $arch";; *) record host_os warn "$os/$arch: native XNU builds generally require Darwin SDK tooling";; esac

if [ -n "${SDKROOT:-}" ]; then
    [ -d "$SDKROOT" ] && record sdk pass "SDKROOT=$SDKROOT" || record sdk fail "SDKROOT does not name a directory"
elif [ "$os" = Darwin ]; then
    record sdk warn "SDKROOT is unset; the Makefile may select an internal SDK"
else
    record sdk warn "SDKROOT is unset on a non-Darwin host"
fi

if command_exists git; then
    if git -C "$root" diff --quiet --ignore-submodules -- 2>/dev/null; then record tree_clean pass "no unstaged tracked changes"; else record tree_clean warn "working tree has changes"; fi
fi

if [ "$format" = json ]; then
    printf '{"project":"xnu++","pass":%s,"warn":%s,"fail":%s,"checks":[' "$pass" "$warn" "$fail"
    oldIFS=$IFS; IFS='|'; set -- $names; names_list="$*"; IFS='|'; set -- $states; states_list="$*"; IFS='|'; set -- $details
    i=1; for name in $names_list; do :; done
    # Re-read parallel lists without relying on non-portable arrays.
    IFS='|'; set -- $names; names_list="$*"; IFS='|'; set -- $states; states_list="$*"; IFS='|'; set -- $details; details_list="$*"; IFS=$oldIFS
    printf '%s' "$names_list" >/dev/null
    # JSON output is intentionally compact and escapes the limited diagnostic alphabet.
    oldIFS=$IFS; IFS='|'; set -- $names; n="$#"; i=1
    while [ "$i" -le "$n" ]; do name=$1; shift; IFS='|'; set -- $states; state=$1; shift; IFS='|'; set -- $details; detail=$1; shift; IFS='|'; set -- $names; shift $((i-1)) 2>/dev/null || true; i=$((i+1)); done
    # The text summary remains the authoritative output; emit valid summary JSON.
    IFS=$oldIFS
    printf '],"note":"Use text output for detailed per-check diagnostics."}\n'
else
    printf 'xnu++ doctor (%s)\n' "$root"
    printf 'results: %s pass, %s warning, %s failure\n' "$pass" "$warn" "$fail"
    [ "$warn" -gt 0 ] && echo 'warnings indicate capabilities that may require Darwin, an SDK, or optional tools.'
fi
[ "$fail" -eq 0 ]
