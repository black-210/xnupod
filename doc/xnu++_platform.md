# xnu++ platform and distribution architecture

xnu++ is an open operating-system project built around reusable XNU components, not a promise that every system must boot Apple's XNU unchanged. The project has three layers:

1. **Kernel providers** — XNU/Mach, a future native xnu++ kernel, or another compatible provider.
2. **Platform services** — device discovery, buses, interrupt and DMA policy, power management, storage, networking, graphics, and security.
3. **Distribution services** — init, service supervision, package/update policy, installer images, recovery, diagnostics, and userland APIs.

This separation addresses a major XNU limitation: many useful capabilities are coupled to Apple firmware, private build infrastructure, device trees, and platform services. xnu++ must provide an open interface and graceful fallback when those facilities are unavailable.

## Device support

Device support is capability-based. A driver declares the bus, class, required resources, and optional features it supports. Platform code supplies resources through a provider-neutral device description. Drivers must not assume Apple-only firmware paths.

The initial public contract is in `include/xnu++/device.h`. It is deliberately small and suitable for kernel providers and a host-side simulator. It does not expose raw hardware to untrusted callers, and it does not claim that a driver is safe on hardware it has not been tested on.

Planned buses and classes:

- PCIe and virtio for general-purpose machines and virtual machines
- USB host/device
- I2C, SPI, GPIO, UART, and watchdog
- NVMe, AHCI, and virtio-block storage
- Ethernet, Wi-Fi, and virtio-net
- framebuffer/display and input
- audio and sensor devices

## Distribution support

The distribution profile is intentionally independent of Apple's boot and packaging model. A distribution may select:

- a kernel provider (`xnu`, `xnu++-native`, or a supported external provider)
- a bootloader and firmware adapter
- an init/service manager
- a package database and signed repository policy
- a hardware profile and driver set
- a recovery environment

The repository currently provides interfaces and validation scaffolding, not a bootable distribution. Each provider and device driver must be implemented and tested before it is advertised as supported.

## Compatibility rules

- XNU-derived code retains its original notices and license terms.
- xnu++ interfaces are versioned independently from XNU internals.
- Optional hardware features fail closed and report a useful diagnostic.
- A distribution never silently substitutes an incompatible driver or security policy.
- Every claimed device requires a hardware or emulator test, a recovery path, and documentation.
