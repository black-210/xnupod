# xnu++ KDE integration boundary

KDE is an optional userspace integration target, not a bootloader component.
The boot manager must remain usable without Qt, KDE, a graphical framebuffer,
or a userspace filesystem. This separation keeps recovery available when the
desktop stack is broken.

## Required integration layers

A future KDE distribution profile must provide:

- a supported kernel/provider and boot handoff;
- init and service supervision;
- graphics device and DRM/KMS-equivalent provider support;
- input, font, audio, network, and storage services;
- a session launcher with least-privilege boundaries;
- package/update metadata with rollback support;
- a recovery target independent of KDE;
- a documented Qt/KDE version and build toolchain.

## Failure behavior

If KDE or the graphics stack fails, the system must retain a serial/text recovery target
and must not mark the boot as fully healthy. The boot manager must not load KDE code,
plugins, or themes as privileged boot artifacts.

## Status

`kde = optional-userspace` in `distribution/boot.profile` records the intended
integration boundary. It does not claim that KDE is currently vendored, built, or
bootable from this XNU tree.
