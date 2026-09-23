# xnu++

xnu++ is a semi-independent operating-system engineering layer built around the XNU codebase and designed to close practical gaps left by upstream Apple distributions. It is not a claim that a complete OS replacement already exists; it is a structured compatibility layer, a distribution model, a device abstraction, a security policy framework, and a reliability platform that can evolve toward a fully independent provider without discarding the robust components already present in XNU.

This repository keeps the upstream XNU tree intact while adding portable tooling, explicit capability detection, provider-neutral interfaces, stronger release policy, recovery logic, and device abstractions. The project is intentionally honest: it distinguishes between what is inherited from XNU, what is adapted, and what is still planned or experimental.

The design philosophy is simple:

- Reuse what is already good and stable.
- Keep compatibility with XNU where it matters.
- Add explicit capability detection instead of guessing platform assumptions.
- Make unsupported devices, firmware paths, and security states fail closed.
- Provide better diagnostics, recovery, rollback, and policy than the default Apple distribution model.
- Create a path toward a future provider-independent xnu++ kernel without pretending that the entire operating system is already independent.

## Why xnu++ exists

XNU is a powerful and mature kernel, but it is not the full story of a modern open operating system. Real distributions require more than Mach, BSD, and IOKit alone:

- robust capability detection and validation
- provider-neutral device abstraction
- secure update and rollback systems
- service isolation and quota enforcement
- recovery-oriented diagnostics
- safer handling of unsupported hardware
- a documented path toward independent platform services

xnu++ addresses these needs by adding a semi-independent layer that can be implemented by XNU today and by a future native xnu++ provider later.

## Project goals

1. Preserve upstream XNU compatibility whenever possible.
2. Remove ambiguity around supported hardware, platform assumptions, and required SDK tools.
3. Add structured diagnostics and recovery instead of silent failure.
4. Introduce provider-neutral interfaces for devices, security, compatibility, and service health.
5. Provide update and recovery policies that are stronger than the default upstream assumptions.
6. Keep the project modular so it remains practical to rebase against future XNU imports.
7. Support a future fully independent provider without breaking XNU compatibility today.

## Architecture summary

xnu++ is organized into three primary layers:

### 1. Kernel providers

This is the source of the actual kernel implementation.

- XNU/Mach/BSD/IOKit provider
- future native xnu++ provider
- alternate compatible providers

A provider is selected explicitly. There is no hidden fallback from a security-sensitive provider to an untrusted one.

### 2. Platform services

These are the components that convert generic capabilities to real hardware support.

- device discovery and classification
- bus abstraction
- interrupt and DMA handling
- power and lifecycle management
- storage and networking support
- compatibility and capability probing
- security verification and audit boundaries

### 3. Distribution services

These are the higher-level system services needed for a complete environment.

- recovery and rollback
- package and update verification
- signed offline recovery
- service supervision and isolation
- diagnostics and crash collection
- safe support matrices and hardware profiles

## Design model: semi-independent by default

The project is intentionally designed as semi-independent rather than fully independent.

This means:

- XNU remains the baseline provider for compatibility and maturity.
- xnu++ adds a compatibility and policy layer that is independent of Apple-private assumptions.
- the same interfaces can later be implemented by a native xnu++ kernel or another provider.
- features are considered supported only when they are tested, documented, and recoverable.

This is a strength, not a weakness. It avoids the lie that a project is independent before it has a tested provider, boot path, recovery story, and hardware support matrix.

## Core features added

The repository now includes a broad set of feature-oriented files and interfaces.

### 1. Project identity and documentation

Files:

- `XNU++_README.md`
- `XNU++_VERSION`
- `doc/xnu++_roadmap.md`
- `doc/xnu++_feature_plan.md`
- `doc/xnu++_platform.md`
- `doc/xnu++_independence.md`
- `doc/xnu++_reliability.md`
- `doc/xnu++_capabilities.md`
- `doc/xnu++_milestones.md`

These define the professional model, roadmap, milestones, and independence boundaries.

### 2. Source-tree validation and diagnostics

Files:

- `tools/xnu++/doctor.sh`
- `tools/xnu++/validate.sh`
- `tools/xnu++/validate-reliability.sh`
- `tools/xnu++/validate-profile.sh`
- `tools/xnu++/capabilities.json`
- `tools/xnu++/support-matrix.json`
- `tools/xnu++/reliability-matrix.json`
- `.github/workflows/xnu++-checks.yml`

These validate:

- the source tree layout
- required directories and files
- host environment and tool availability
- SDK awareness and Darwin assumptions
- compatibility metadata
- device support reporting
- reliability profile and update policy validation

The doctor script is deliberately conservative and read-only. It reports warnings for optional capabilities and failures for conditions that make the requested workflow unreliable.

### 3. Compatibility and capability model

Files:

- `include/xnu++/compat.h`
- `include/xnu++/security.h`
- `include/xnu++/device.h`
- `include/xnu++/bus.h`
- `include/xnu++/reliability.h`
- `include/xnu++/features.h`
- `include/xnu++/update.h`
- `include/xnu++/recovery.h`
- `include/xnu++/quota.h`
- `include/xnu++/isolation.h`
- `include/xnu++/diagnostics.h`

These provide provider-neutral capability checks and policy contracts. They allow the system to detect supported features without hard-coding Apple-only platform strings.

### 4. Security and protection systems

The security layer provides fail-closed semantics and is intentionally stricter than default permissive behavior.

Security features include:

- secure boot policy
- measured boot verification
- signed driver enforcement
- update-signature validation
- rollback protection
- least privilege enforcement
- IOMMU requirement
- authenticated offline recovery
- audit of privileged operations
- deny-by-default policy for unknown devices and unknown capabilities

Relevant files:

- `include/xnu++/security.h`
- `distribution/security.profile`
- `tools/xnu++/support-matrix.json`

Security contract behavior:

- Unknown primitives must not be accepted silently.
- Malformed requests are denied.
- Security-sensitive provider failover is not allowed.
- A recovery path is mandatory before a system may claim a secure state.

### 5. Device support and platform abstraction

This is one of the most important additions to xnu++.

Files:

- `include/xnu++/device.h`
- `include/xnu++/bus.h`
- `doc/xnu++_platform.md`
- `distribution/default.profile`

Device support model:

- provider-neutral device IDs and compatibility strings
- state tracking for detach/discover/attach/suspend/fail
- resource descriptors for MMIO, I/O, IRQ, DMA, clocks, and resets
- explicit bus abstraction
- safe lifecycle behavior and fail-closed handling

Supported and planned classes include:

- PCIe
- virtio
- USB
- I2C
- SPI
- UART
- GPIO
- watchdog
- NVMe
- AHCI
- virtio-block
- virtio-net
- Ethernet
- Wi-Fi
- framebuffer/display
- input devices
- audio
- sensors

The device model does not assume Apple firmware or Apple-only boot paths. Devices must declare requirements and compatibility explicitly, and a distribution must reject unverified devices rather than silently accepting them.

### 6. Reliability and recovery model

Files:

- `include/xnu++/reliability.h`
- `include/xnu++/features.h`
- `include/xnu++/update.h`
- `include/xnu++/recovery.h`
- `include/xnu++/quota.h`
- `include/xnu++/isolation.h`
- `include/xnu++/diagnostics.h`
- `distribution/reliability.profile`
- `distribution/update.policy`
- `distribution/recovery.policy`

These bring xnu++ much closer to a serious production-safe layer.

Key capabilities:

- provider health monitoring and quarantine
- failover policy for safe services only
- rollbacks for failed updates
- quota enforcement for CPU, memory, I/O, and handles
- service isolation and least-privilege enforcement
- structured diagnostics with severity levels
- checkpoint-informed recovery planning
- signed offline recovery and authenticated state reset

### 7. Distribution profiles and policy files

Files:

- `distribution/default.profile`
- `distribution/security.profile`
- `distribution/reliability.profile`
- `distribution/update.policy`
- `distribution/recovery.policy`

These create a policy-based distribution model instead of relying on Apple-specific assumptions.

Profiles define required capabilities such as:

- secure boot
- measured boot
- signed updates
- rollback protection
- offline recovery
- IOMMU
- service isolation
- capability registry
- explicit hardware support list

## Device support matrix

xnu++ uses capability-based support reporting. A device is not considered supported just because a bus exists. Support is split into explicit statuses:

- supported
- interface-defined
- planned
- emulator-tested
- provider-specific
- unsupported

Example matrix categories:

- virtio block and networking: emulator-tested
- PCIe and USB: interface-defined
- I2C and SPI: interface-defined
- NVMe, Ethernet, audio, Wi-Fi, display backends: planned or provider-defined

The system is designed to deny unknown or unverified devices by policy.

## Professional guidance: semi-independent but extendable

xnu++ is designed so that it can be fully independent later without forcing a disruptive rewrite now.

That future path includes:

- native xnu++ driver model
- native distributed boot and recovery infrastructure
- self-hosted update pipeline
- fully provider-neutral security and device stacks
- a stronger kernel provider that is fully independent of Apple firmware assumptions

At the same time, the project remains safe and realistic:

- it does not claim to support unsupported hardware
- it does not guess platform capabilities
- it does not silently bypass security checks
- it does not advertise features without tests or documentation

## Practical usage

From the repository root, you can run:

```sh
sh tools/xnu++/doctor.sh
```

For machine-readable output:

```sh
sh tools/xnu++/doctor.sh --format json
```

Validation:

```sh
sh tools/xnu++/validate.sh
```

Reliability validation:

```sh
sh tools/xnu++/validate-reliability.sh
```

Profile validation:

```sh
sh tools/xnu++/validate-profile.sh
```

## Repository structure

A representative project structure is:

```text
.
├── README.md
├── XNU++_README.md
├── XNU++_VERSION
├── Makefile
├── APPLE_LICENSE
├── EXTERNAL_HEADERS/
├── SETUP/
├── bsd/
├── config/
├── doc/
├── include/
├── distribution/
├── tools/
├── osfmk/
├── pexpert/
├── security/
├── tests/
└── tools/xnu++/
```

## Operational principles

The xnu++ layer enforces a set of principles that are crucial for reliability and safety:

- Security-sensitive operations never fail over to an untrusted provider.
- Unknown required capabilities are denied.
- Unsupported hardware and missing firmware paths are treated as explicit failures, not assumptions.
- Optional features degrade gracefully with a report.
- Updates are signed, verified, and rollback-capable.
- Diagnostics are structured and auditable.
- Device support is declared and validated instead of guessed.

## Final statement

xnu++ is not a lie, a fake replacement, or an unrealistic claim. It is a serious engineering layer that strengthens XNU-derived workflows by making them more portable, safer, more diagnosable, and more capable of future independence.

It combines:

- upstream XNU stability
- explicit compatibility detection
- provider-neutral device and bus contracts
- independent security and recovery policy
- structured update and rollback handling
- robust diagnostics and support matrices
- future-ready independence without reckless abandonment of proven kernel technology

This makes xnu++ not just a patch set, but a structured, extensible platform architecture for a more reliable and open operating system story around XNU.
