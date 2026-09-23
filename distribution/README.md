# xnu++ distribution profile

This directory describes a distribution-oriented build profile without coupling the project to Apple's private SDKs or boot infrastructure.

## Provider selection

A distribution must select one kernel provider explicitly:

- `xnu`: the imported XNU provider, subject to its platform and SDK requirements
- `xnu++-native`: future xnu++ kernel implementation
- `external`: a separately maintained compatible provider

There is no implicit fallback from a failed privileged provider to a less secure one.

## Required distribution components

A usable image needs all of the following:

- boot and recovery path
- kernel provider
- device and bus drivers
- init and service supervision
- filesystem and storage support
- networking configuration
- package/update mechanism
- signed metadata and rollback policy
- crash collection and recovery diagnostics

The repository currently defines interfaces and validation rules for these components. It does not yet ship a complete image.

## Device profile example

A profile may list required and optional devices:

```text
required = pci, nvme, ethernet, uart
optional = usb, framebuffer, audio, wifi
provider = xnu
recovery = serial
```

A boot or installation tool must reject a profile when a required device has no tested driver. Optional devices are reported as unavailable rather than emulated unsafely.
