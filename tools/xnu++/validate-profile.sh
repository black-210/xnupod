#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
[ -f "$root/distribution/security.profile" ]
[ -f "$root/tools/xnu++/support-matrix.json" ]
if command -v python3 >/dev/null 2>&1; then
  python3 - "$root/tools/xnu++/support-matrix.json" <<'PY'
import json, sys
p=json.load(open(sys.argv[1], encoding='utf-8'))
assert p['schema'] == 1
assert p['policy']['unverified_devices'] == 'deny'
assert p['devices']
PY
fi
printf '%s\n' 'xnu++ profiles passed validation'
