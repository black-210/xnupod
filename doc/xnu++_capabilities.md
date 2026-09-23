# xnu++ capability registry

The registry gives userland, boot, and platform services one versioned description of what a provider can do. It is intentionally additive: missing optional capabilities must result in a documented fallback, while missing required capabilities must stop the operation before it mutates state.

## Capability classes

- `kernel`: Mach IPC, VM, scheduling, syscall and tracing services
- `device`: PCIe, virtio, USB, I²C, SPI, NVMe and networking
- `security`: measured boot, IOMMU, signed modules, rollback protection
- `reliability`: watchdog, crash persistence, checkpoint/restore, failover
- `distribution`: recovery, transactional updates, package verification

Provider reports should include an API version, implementation identifier, capability bits, and limitations. Consumers must compare versions and reject unknown mandatory policy requirements.
