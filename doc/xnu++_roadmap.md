# xnu++ roadmap

This roadmap turns the broad goal of “fill the gaps in XNU” into work that can be tested and reviewed. XNU is a kernel, not a normal portable application: some interfaces depend on Apple firmware, private SDKs, device drivers, or platform security hardware. xnu++ therefore distinguishes **portable improvements** from **platform integrations** instead of promising unsupported compatibility.

## Phase 1 — reliable development (current)

- Detect host, compiler, SDK, architecture, and source-tree prerequisites without changing the tree.
- Document the boundary between upstream XNU code and xnu++ additions.
- Add CI checks for shell portability, repository layout, generated-file hygiene, and documentation links.
- Establish a repeatable build matrix for Darwin hosts where an SDK is available.

## Phase 2 — compatibility surface

- Add a versioned capability manifest for architecture, VM, IPC, security, and driver features.
- Provide compile-time feature probes rather than platform-name conditionals.
- Improve error reporting for missing SDK headers, unsupported compiler flags, and unavailable firmware interfaces.
- Add host-side tests for parsers and tooling, especially KCDATA/libkdd consumers.

## Phase 3 — resilience and observability

- Add opt-in structured diagnostics for boot, panic, memory-pressure, and scheduler paths.
- Expand kernel POST coverage for boundary conditions and cleanup paths.
- Add fuzz targets for user-controlled parsers and KCDATA readers, with bounded resource usage.
- Define safe failure behavior for optional facilities: a missing accelerator or driver must not silently corrupt kernel state.

## Phase 4 — platform adapters

- Keep architecture-specific code behind narrow interfaces and capability checks.
- Develop adapters only where hardware, firmware, and licensing permit; do not emulate privileged Apple interfaces in userspace and call it kernel support.
- Track each adapter against a documented target Darwin/XNU release.
- Contribute generally useful fixes upstream when they do not depend on xnu++ policy.

## Contribution rules

1. Every feature needs a test or a documented reason a test cannot run off-target.
2. No new global ABI is introduced without a versioning and compatibility story.
3. Security-sensitive changes need threat-model notes and negative tests.
4. Imported XNU code keeps its original copyright and license notices.
5. Prefer small, reviewable commits over broad mechanical renames; the project name is xnu++, but upstream synchronization must remain practical.
