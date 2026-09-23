# xnu++

xnu++ is the compatibility and engineering layer for this xnu-derived tree. It keeps the upstream XNU kernel sources intact while adding portable tooling, explicit capability detection, and a place to land improvements that close practical gaps around building, testing, and documenting XNU.

## Project goals

- Preserve source compatibility with upstream XNU wherever possible.
- Make build prerequisites and unsupported host/platform combinations explicit instead of failing mysteriously.
- Provide reproducible, inspectable development workflows before attempting a kernel build.
- Grow portable tests and adapters without claiming that a non-Apple host can boot an Apple kernel.
- Keep changes modular so they can be upstreamed or rebased against newer XNU imports.

## Current additions

- `tools/xnu++/doctor.sh` performs a read-only environment and source-tree diagnosis.
- `doc/xnu++_roadmap.md` defines the gap-closing plan and compatibility boundaries.
- `XNU++_VERSION` gives downstream automation a stable project identity.

Run the doctor from the repository root:

```sh
sh tools/xnu++/doctor.sh
```

For machine-readable output:

```sh
sh tools/xnu++/doctor.sh --format json
```

The doctor is intentionally conservative: warnings identify missing optional capabilities, while failures identify conditions that make the requested workflow unreliable. It does not download dependencies, modify the tree, or attempt to boot a kernel.

## Relationship to XNU

xnu++ is not a replacement kernel and does not imply compatibility with every Darwin release. This repository remains derived from Apple Open Source XNU and retains its upstream licensing and notices. New functionality must be clearly separated from imported code and must respect the licenses of all dependencies.
