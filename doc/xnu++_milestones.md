# xnu++ implementation milestones

## Device independence

- [x] Define a provider-neutral device/driver contract.
- [x] Define explicit device states and safe suspend/resume hooks.
- [ ] Implement a host-side simulator for resource and lifecycle tests.
- [ ] Add PCIe and virtio discovery providers.
- [ ] Add storage and network drivers with emulator coverage.
- [ ] Add hardware compatibility reports generated from tested profiles.

## Reduced dependence on XNU

- [x] Document kernel-provider boundaries.
- [ ] Move portable parsers, diagnostics, and policy code behind provider-neutral APIs.
- [ ] Implement a minimal native scheduler, VM, IPC, and syscall prototype in a separate provider.
- [ ] Keep the native provider bootable first in a virtual machine before targeting physical hardware.
- [ ] Define ABI compatibility and migration tools for userland.

## Distribution readiness

- [x] Add a distribution profile format and required-component policy.
- [ ] Build a reproducible image pipeline.
- [ ] Add init/service supervision and recovery mode.
- [ ] Add signed package metadata, rollback, and offline installation.
- [ ] Publish supported hardware and virtualization matrices.

The project should call something “supported” only after automated tests, a recovery path, and documentation exist. Open source alone does not remove firmware, hardware, licensing, or security constraints.
