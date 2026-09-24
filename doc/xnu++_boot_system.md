# xnu++ boot system architecture

This document defines the first reviewable boot-platform slice for xnu++. It is an architecture and policy boundary, not a claim that this repository already contains a production firmware implementation or a complete operating-system image.

## Scope

The boot platform is divided into explicit stages:

1. **Firmware** — UEFI, Apple firmware, or another documented provider owns initial CPU and memory setup.
2. **Boot manager** — discovers a supported boot volume, reads the signed manifest, verifies policy, presents the boot UI, and selects a kernel entry.
3. **Boot services** — loads the kernel, auxiliary drivers, device tree/ACPI data, boot arguments, and measured-boot records.
4. **Kernel provider** — transfers control to the selected XNU provider and passes a versioned boot handoff.
5. **Recovery environment** — provides diagnostics, rollback, and authenticated repair when normal boot cannot proceed.
6. **Desktop session** — a future userspace layer; KDE is an optional integration target and is never part of the privileged boot chain.

The boot manager must fail closed when a required artifact, capability, signature, or handoff contract is missing. It must not silently substitute an unverified kernel or driver.

## Handoff contract

A future implementation must pass a versioned handoff containing at least:

- architecture and firmware/provider identity
- memory map and reserved ranges
- framebuffer or display protocol information, if available
- ACPI/device-tree root and validated bounds
- boot volume identity and filesystem metadata
- kernel, auxiliary collection, and driver measurements
- boot arguments and their provenance
- secure-boot and recovery state
- monotonic boot attempt and rollback counters

The handoff is an ABI. Changes require a schema version and compatibility rules. Unknown required fields are fatal; unknown optional fields may be ignored only when policy permits.

## Verification order

The boot manager should perform checks in this order:

1. parse and bounds-check the manifest;
2. verify the manifest signature against the configured trust anchor;
3. enforce rollback and boot-attempt policy;
4. validate architecture, provider, and required capabilities;
5. verify hashes and signatures for every loadable artifact;
6. reserve memory and construct the handoff;
7. record measurements and boot status;
8. display the boot menu or use the configured default;
9. load artifacts and transfer control.

Failure at any step enters the recovery path with a diagnostic reason. Recovery must not report a secure state unless its own authentication and policy checks pass.

## UI boundary

The boot UI is intentionally separate from verification. It may show:

- signed boot entries and their version/build identifiers
- verification and recovery status
- hardware/provider capability results
- diagnostics and safe-mode choices
- rollback and recovery actions permitted by policy

The UI must not be able to disable signature verification, modify trust anchors, or grant privileged fallback access. A text/serial UI is mandatory for recovery; a graphical UI is an optional presentation layer.

## Implementation status

This repository currently provides policy and prototype tooling only. Firmware-specific loaders, filesystem drivers, cryptographic key provisioning, kernel handoff assembly, and hardware validation remain provider-specific implementation work. They must be added only with tests, threat-model notes, and a documented support matrix.
