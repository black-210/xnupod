#!/bin/sh
# Validate the ten-file reliability expansion without requiring an Apple SDK.
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
for path in \
  include/xnu++/update.h include/xnu++/recovery.h include/xnu++/quota.h \
  include/xnu++/isolation.h include/xnu++/diagnostics.h \
  doc/xnu++_independence.md distribution/update.policy distribution/recovery.policy \
  tools/xnu++/reliability-matrix.json; do
  [ -f "$root/$path" ] || { echo "missing: $path" >&2; exit 1; }
done
if command -v cc >/dev/null 2>&1; then
  tmp=${TMPDIR:-/tmp}/xnuxx-header-check.$$
  trap 'rm -f "$tmp" "$tmp.c"' EXIT HUP INT TERM
  cat > "$tmp.c" <<'C'
#include "include/xnu++/update.h"
#include "include/xnu++/recovery.h"
#include "include/xnu++/quota.h"
#include "include/xnu++/isolation.h"
#include "include/xnu++/diagnostics.h"
int main(void) { return 0; }
C
  cc -std=c11 -Wall -Wextra -Werror -I"$root" "$tmp.c" -o "$tmp"
fi
printf '%s\n' 'xnu++ reliability expansion passed validation'
