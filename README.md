# xnu++

xnu++ is an experimental, semi-independent operating-system platform and systems-engineering project built around the architectural foundations of XNU, Mach, BSD, and IOKit.

The project is designed to provide a practical platform layer that can be used as the foundation for operating-system distributions and specialized systems.

One such distribution is Orange OS, a CLI-first operating-system distribution built on top of xnu++.

«Project status: Experimental / active development
Primary architectures: i386 and x86-64
Boot protocol: Multiboot2
Bootloader: Limine
Development environment: Freestanding C, LLVM/Clang, LLD, QEMU
Desktop target: KDE Plasma userspace integration
Primary language: C»

---

Table of Contents

- "Overview" (#overview)
- "What Is xnu++?" (#what-is-xnu)
- "Project Goals" (#project-goals)
- "Architecture" (#architecture)
- "xnu++ and Orange OS" (#xnuxx-and-orange-os)
- "Kernel and Platform Layer" (#kernel-and-platform-layer)
- "Boot Process" (#boot-process)
- "Multiboot2" (#multiboot2)
- "i386 Support" (#i386-support)
- "x86-64 Support" (#x86-64-support)
- "Memory and Low-Level Facilities" (#memory-and-low-level-facilities)
- "Console and VGA" (#console-and-vga)
- "GDT and IDT" (#gdt-and-idt)
- "PIC and Interrupt Handling" (#pic-and-interrupt-handling)
- "Keyboard Input" (#keyboard-input)
- "Provider Architecture" (#provider-architecture)
- "Security and Boot Verification" (#security-and-boot-verification)
- "Recovery and Rollback Architecture" (#recovery-and-rollback-architecture)
- "Device and Bus Architecture" (#device-and-bus-architecture)
- "Orange OS" (#orange-os)
- "Orange OS Userspace" (#orange-os-userspace)
- "Orange OS Filesystem Layout" (#orange-os-filesystem-layout)
- "Orange OS Configuration" (#orange-os-configuration)
- "Orange CLI" (#orange-cli)
- "Package Management" (#package-management)
- "Service Management" (#service-management)
- "KDE Plasma" (#kde-plasma)
- "Build System" (#build-system)
- "Building xnu++" (#building-xnuxx)
- "Building Orange OS" (#building-orange-os)
- "Running with QEMU" (#running-with-qemu)
- "ISO Images" (#iso-images)
- "Repository Structure" (#repository-structure)
- "Development Model" (#development-model)
- "Current Implementation Status" (#current-implementation-status)
- "Experimental Components" (#experimental-components)
- "Planned Components" (#planned-components)
- "Design Principles" (#design-principles)
- "Limitations" (#limitations)
- "Security Status" (#security-status)
- "Roadmap" (#roadmap)
- "Contributing" (#contributing)
- "License" (#license)

---

Overview

xnu++ is an experimental operating-system engineering project focused on building a practical platform around XNU-oriented architecture while providing its own interfaces, boot infrastructure, security architecture, device abstractions, recovery concepts, diagnostics, and distribution support.

The project is intentionally separated into two major layers:

                    xnu++
             Kernel / Platform
                      │
        ┌─────────────┴─────────────┐
        │                           │
     Platform                  System Services
        │                           │
        └─────────────┬─────────────┘
                      │
                  Userspace
                      │
                 Orange OS
                      │
             Applications / CLI
                      │
                KDE Plasma
                 (optional)

The purpose of this separation is to avoid turning the kernel/platform into a desktop environment.

xnu++ provides the underlying platform.

Orange OS provides a distribution and userspace environment.

KDE Plasma is treated as a userspace desktop environment rather than part of the kernel.

---

What Is xnu++?

xnu++ is not intended to simply be a renamed copy of XNU.

The project is designed around a layered architecture in which existing technologies can act as providers while xnu++ supplies a common platform-oriented interface.

The conceptual model is:

Applications
     │
     ▼
Userspace
     │
     ▼
System Services
     │
     ▼
xnu++ Platform Layer
     │
 ┌───┼───────────────────────┐
 │   │                       │
 ▼   ▼                       ▼
XNU Native               Device/Bus
Provider Provider        Abstractions
 │
 ▼
Mach / BSD / IOKit

This allows the project to evolve without requiring every component to be implemented simultaneously.

---

Project Goals

The major goals of xnu++ are:

- Build an independent systems platform around XNU-oriented architecture.
- Provide provider-neutral interfaces.
- Support multiple kernel/platform implementations.
- Provide a controlled boot process.
- Provide security-policy interfaces.
- Provide recovery and rollback infrastructure.
- Provide device and bus abstractions.
- Provide capability probing.
- Provide diagnostics.
- Provide isolation and resource-management concepts.
- Provide distribution support.
- Provide a foundation for operating-system distributions.
- Support both 32-bit and 64-bit x86 development.
- Provide a practical environment for systems programming research.

The project also aims to make it possible to build a distribution without placing distribution-specific logic directly inside the kernel.

---

Architecture

The high-level architecture is:

┌───────────────────────────────────────────────┐
│                 Applications                  │
├───────────────────────────────────────────────┤
│                 Userspace                     │
├───────────────────────────────────────────────┤
│              System Services                 │
├───────────────────────────────────────────────┤
│                Orange OS                     │
│       CLI / Packages / Services / Config      │
├───────────────────────────────────────────────┤
│                    xnu++                      │
│          Kernel / Platform / Boot             │
├───────────────────────────────────────────────┤
│       Provider / Device / Security APIs       │
├───────────────────────────────────────────────┤
│          XNU / Mach / BSD / IOKit             │
└───────────────────────────────────────────────┘

The architecture is intentionally modular.

---

xnu++ and Orange OS

Orange OS is a separate project and repository.

It is designed as a distribution built on top of xnu++.

The relationship is:

xnu++
  │
  ├── Kernel
  ├── Boot infrastructure
  ├── Security infrastructure
  ├── Device layer
  ├── Recovery infrastructure
  └── Platform interfaces
          │
          ▼
      Orange OS
          │
          ├── init
          ├── CLI
          ├── Package management
          ├── Service management
          ├── Configuration
          ├── Userspace
          └── Desktop integration

Orange OS does not provide a second kernel.

It uses xnu++ as its underlying platform.

This distinction is important:

xnu++     = platform/kernel
Orange OS = distribution/userspace
KDE       = desktop/userspace component

---

Kernel and Platform Layer

The kernel/platform layer currently contains several low-level components.

These include:

- boot entry
- platform initialization
- VGA console
- low-level terminal output
- port I/O
- CPU interrupt control
- GDT initialization
- IDT infrastructure
- PIC infrastructure
- keyboard polling
- memory allocation
- memory tests
- panic handling
- provider selection
- boot verification interfaces
- boot capability interfaces

The implementation is currently designed for freestanding compilation.

The kernel does not depend on a normal host operating-system libc.

---

Boot Process

The x86-64 boot path currently uses:

Firmware / BIOS
       │
       ▼
     Limine
       │
       ▼
   Multiboot2
       │
       ▼
multiboot2_64.S
       │
       ▼
   Long Mode
       │
       ▼
   boot_entry()
       │
       ▼
boot_platform_start()
       │
       ▼
   kernel_main()

The 64-bit boot assembly performs the initial CPU transition into long mode.

The boot path includes:

- disabling interrupts
- storing Multiboot2 information
- establishing a temporary stack
- creating initial paging structures
- loading CR3
- enabling PAE
- enabling long mode through EFER
- enabling paging
- loading a 64-bit GDT
- switching to 64-bit code
- establishing a 64-bit stack
- calling the C boot entry

---

Multiboot2

xnu++ uses Multiboot2 as its kernel boot protocol.

The kernel contains a Multiboot2 header and receives the Multiboot2 information structure from the bootloader.

The boot entry currently receives:

void boot_entry(uint32_t magic, uint32_t multiboot_info);

The Multiboot2 bootloader magic value is validated before continuing.

The current implementation is being extended toward using Multiboot2 modules for userspace components such as Orange OS.

The intended architecture is:

Multiboot2
   │
   ├── xnu++ kernel
   │
   └── Orange OS init

This provides a clean path toward passing userspace payloads from the boot environment into the platform.

---

i386 Support

xnu++ currently includes an i386 build.

The resulting kernel is:

build/xnuxx.elf

The i386 implementation contains:

- 32-bit boot entry
- Multiboot2 support
- VGA text console
- GDT
- IDT
- PIC
- keyboard input
- memory allocation
- kernel diagnostics
- provider infrastructure

The project intentionally retains i386 support instead of replacing it with x86-64.

---

x86-64 Support

x86-64 support was added as a parallel platform.

The resulting kernel is:

build64/xnuxx.elf

The x86-64 boot path contains:

boot/multiboot2_64.S
boot/linker64.ld
kernel/kernel64.c

The bootloader enters 64-bit long mode before transferring control to the C kernel.

The x86-64 kernel is currently an experimental bring-up implementation.

Its purpose is to establish a working 64-bit platform on which additional xnu++ functionality can be developed.

---

Memory and Low-Level Facilities

The kernel includes a simple early memory allocator intended for low-level bring-up.

The allocator is deliberately simple and is not intended to replace a complete production memory-management subsystem.

Current concepts include:

- heap start address
- heap size
- alignment
- bump allocation
- allocation testing
- basic memory diagnostics

The current allocator does not provide a complete reclamation system.

Future memory-management work includes:

- page allocation
- physical memory management
- virtual memory
- paging abstraction
- memory zones
- allocation policies
- address-space management
- isolation
- quotas

---

Console and VGA

The current x86 implementation provides a VGA text-mode console.

The standard VGA memory address is:

0xB8000

The console supports basic operations such as:

- character output
- strings
- newline handling
- hexadecimal output
- integer output
- screen clearing
- cursor management
- scrolling

This console is primarily intended for early boot diagnostics and kernel development.

---

GDT and IDT

xnu++ contains Global Descriptor Table infrastructure.

The GDT is used to establish kernel code and data segments.

The initial configuration includes:

Null descriptor
Kernel code
Kernel data

The x86-64 implementation uses 64-bit GDT descriptors.

The project also contains Interrupt Descriptor Table infrastructure.

The IDT provides the foundation for:

- CPU exceptions
- hardware interrupts
- future syscall entry
- interrupt dispatch

The 64-bit interrupt implementation is still being expanded.

---

PIC and Interrupt Handling

The i386 implementation includes legacy PIC support.

The primary PIC ports are:

0x20
0x21

The secondary PIC ports are:

0xA0
0xA1

Keyboard hardware uses:

0x60
0x64

The current interrupt architecture is primarily an early bring-up implementation.

Future work includes:

- complete x86-64 interrupt handling
- APIC support
- LAPIC
- IOAPIC
- timer abstraction
- interrupt routing
- SMP support

---

Keyboard Input

The kernel includes basic PS/2 keyboard polling support.

The implementation uses a US keyboard map and filters key-release events.

This provides basic early console input without requiring a complete userspace input stack.

Future work includes a proper device-driver model and input subsystem.

---

Provider Architecture

One of the central architectural ideas in xnu++ is the provider model.

The platform can expose common interfaces while allowing different implementations underneath.

Conceptually:

                 xnu++ API
                    │
          ┌─────────┴─────────┐
          │                   │
          ▼                   ▼
      XNU Provider       Native Provider
          │                   │
          ▼                   ▼
    XNU / Mach / BSD      xnu++ Native

The current implementation defines provider identifiers including:

XNUXX_PROVIDER_XNU
XNUXX_PROVIDER_NATIVE

The kernel can prefer an XNU provider when one is available and fall back to the native implementation.

This architecture allows the project to evolve incrementally.

---

Security and Boot Verification

xnu++ contains a boot-security interface.

The bootloader API supports concepts including:

- signature verification
- SHA-256 measurement
- rollback protection
- capability requirements
- recovery
- kernel loading

The boot API defines policies such as:

XNUXX_BOOT_REQUIRE_SIGNATURE
XNUXX_BOOT_REQUIRE_MEASUREMENT
XNUXX_BOOT_REQUIRE_ROLLBACK_PROTECTION
XNUXX_BOOT_REQUIRE_IOMMU
XNUXX_BOOT_REQUIRE_LAYER_CAPABILITIES
XNUXX_BOOT_STRICT

The boot image structure contains fields for:

- magic
- version
- header size
- generation
- required capabilities
- payload
- payload size
- signature
- signature size
- expected measurement

Important security status

The current boot-platform implementation contains bring-up stubs.

For example, the current signature-verification callback does not implement production cryptographic verification.

The SHA-256 measurement callback is also currently a placeholder.

Therefore xnu++ does not currently claim to provide production secure boot.

The architecture exists so real cryptographic implementations can be integrated later.

This distinction is intentional.

---

Recovery and Rollback Architecture

The boot interface includes recovery and rollback concepts.

A boot image can specify a generation number.

The platform can provide a minimum accepted generation.

This provides the architectural foundation for rejecting older images.

Recovery support is also represented by:

enter_recovery()

The current implementation is experimental.

Future versions are expected to integrate:

- persistent boot state
- boot slots
- generation counters
- recovery environments
- atomic updates
- rollback
- integrity verification
- failed-boot recovery

---

Device and Bus Architecture

xnu++ is designed to eventually provide provider-neutral device and bus interfaces.

The intended abstraction is:

Applications
      │
Userspace
      │
System services
      │
Device abstraction
      │
Bus abstraction
      │
Provider
      │
Hardware

The architecture is intended to prevent hardware-specific implementation details from leaking into distribution code.

Planned areas include:

- PCI
- PCIe
- USB
- storage
- networking
- input
- display
- audio
- platform devices

---

Orange OS

Orange OS is the first distribution-oriented userspace project built on top of xnu++.

Orange OS is designed to be:

- CLI-first
- minimal
- modular
- configurable
- distribution-oriented
- suitable for experimentation
- capable of supporting a desktop environment later

Orange OS currently contains:

CLI
Package management
Service management
Configuration
Userspace init
Root filesystem
Build output
ISO generation

Orange OS is maintained separately from xnu++.

---

Orange OS Userspace

The Orange OS userspace contains a freestanding initialization program.

The source is:

userspace/init.c

The resulting binaries are:

build/orange_init.elf
build64/orange_init.elf

The i386 binary is:

ELF 32-bit i386

The x86-64 binary is:

ELF 64-bit x86-64

The current init performs basic early userspace initialization and displays Orange OS information.

The long-term goal is to turn this into a proper userspace initialization environment responsible for starting system services.

---

Orange OS Filesystem Layout

The current Orange OS filesystem structure is:

orange-os/
├── bin/
│   └── orange
│
├── docs/
│   └── ARCHITECTURE.md
│
├── etc/
│   ├── orange/
│   │   └── services/
│   └── orange.conf
│
├── packages/
│
├── rootfs/
│   ├── bin/
│   │   ├── init
│   │   └── orange
│   │
│   ├── dev/
│   ├── etc/
│   │   ├── init.conf
│   │   ├── orange.conf
│   │   └── os-release
│   │
│   ├── home/
│   ├── proc/
│   ├── root/
│   ├── run/
│   ├── sys/
│   ├── tmp/
│   │
│   ├── usr/
│   │   ├── bin/
│   │   └── lib/
│   │
│   └── var/
│       ├── lib/orange/
│       └── log/
│
├── system/
├── userspace/
│   └── init.c
│
├── var/
│   └── lib/orange/
│
├── build/
│
├── build64/
│
├── Makefile
└── README.md

---

Orange OS Configuration

The main configuration file is:

etc/orange.conf

The current configuration includes:

NAME="Orange OS"
VERSION="0.2.0"
PLATFORM="xnu++"
MODE="cli"

PREFIX="/usr/local"

STATE_ROOT="/var/lib/orange"
PACKAGE_ROOT="/var/lib/orange/packages"
PACKAGE_DB="/var/lib/orange/packages.db"
LOG_ROOT="/var/log/orange"

SERVICE_ROOT="/etc/orange/services"
SERVICE_STATE="/var/lib/orange/services"

DESKTOP="kde-plasma"
DISPLAY_SERVER="wayland"

This separates distribution policy from the kernel.

---

Orange CLI

The main Orange command is:

orange

The CLI provides commands for:

orange init
orange system
orange package
orange service
orange version
orange help

Package-related operations include:

list
search
install
remove
info
update

Service-related operations include:

list
enable
disable
start
stop
status

The CLI is implemented as a shell-based management layer during the early distribution stage.

It can later be replaced or supplemented with a native implementation.

---

Package Management

Orange OS contains an early package-management architecture.

The current package system maintains package metadata and state under:

/var/lib/orange/

The package database is:

/var/lib/orange/packages.db

The package root is:

/var/lib/orange/packages

The current implementation is intentionally simple.

Future package-management work includes:

- package metadata
- dependency resolution
- version constraints
- repositories
- package signatures
- package integrity
- atomic installation
- transaction rollback
- package verification
- offline repositories
- distribution profiles

---

Service Management

Orange OS includes an early service-management system.

Service definitions are stored under:

/etc/orange/services

Service state is stored under:

/var/lib/orange/services

A service can describe:

NAME
DESCRIPTION
COMMAND

The current service layer is experimental and intended to establish the distribution interface before a complete service supervisor is implemented.

---

KDE Plasma

Orange OS is designed to support KDE Plasma as a userspace desktop environment.

The configuration currently identifies:

DESKTOP="kde-plasma"
DISPLAY_SERVER="wayland"

The intended architecture is:

xnu++
   ↓
Orange OS
   ↓
Userspace
   ↓
Wayland
   ↓
KDE Plasma

KDE Plasma is not part of the xnu++ kernel.

The desktop environment belongs to the distribution/userspace layer.

The current KDE entry is a declared integration target rather than a claim that a complete production KDE environment is already shipped.

---

Build System

The project is designed to use LLVM tooling.

Primary tools include:

clang
ld.lld
xorriso
QEMU
Limine

xnu++ is compiled in freestanding mode.

Typical flags include:

-ffreestanding
-fno-pic
-fno-stack-protector
-fno-builtin

This avoids depending on a normal host userspace runtime.

---

Building xnu++

The i386 build produces:

build/xnuxx.elf

The x86-64 build produces:

build64/xnuxx.elf

The project intentionally keeps both architectures.

A typical x86-64 linker invocation uses:

ld.lld -m elf_x86_64

The x86-64 linker script is:

boot/linker64.ld

The x86-64 boot assembly is:

boot/multiboot2_64.S

---

Building Orange OS

Orange OS provides two initialization binaries:

build/orange_init.elf
build64/orange_init.elf

The first is i386.

The second is x86-64.

Both are freestanding ELF binaries.

Orange OS can therefore maintain parallel architecture builds without replacing the existing i386 implementation.

---

Running with QEMU

xnu++ can be tested using QEMU.

The x86-64 development environment uses:

qemu-system-x86_64

For terminal-based VGA testing, a curses display can be used:

qemu-system-x86_64 \
    -cdrom xnuxx-x86_64.iso \
    -boot d \
    -m 256M \
    -display curses \
    -no-reboot \
    -no-shutdown

The same approach can be used for the Orange OS development ISO.

A graphical QEMU display may depend on the host's SDL/GTK/Mesa environment.

---

ISO Images

The development environment uses Limine and xorriso to construct bootable ISO images.

The Orange OS development ISO contains:

build/iso/
├── boot/
│   ├── limine/
│   │   ├── limine-bios-cd.bin
│   │   └── limine-bios.sys
│   │
│   ├── xnuxx.elf
│   └── orange_init.elf
│
└── limine.conf

The configuration identifies xnu++ as the Multiboot2 kernel and Orange init as a module.

Conceptually:

Orange OS ISO
      │
      ▼
    Limine
      │
      ▼
   xnu++ ELF
      │
      └── Orange init module

The current ISO generation is primarily a development and bring-up mechanism.

---

Repository Structure

The xnu++ repository currently contains major areas such as:

xnupp/
├── boot/
│   ├── boot_entry.c
│   ├── boot_platform.c
│   ├── linker.ld
│   ├── linker64.ld
│   ├── multiboot2.S
│   └── multiboot2_64.S
│
├── kernel/
│   ├── kernel.c
│   └── kernel64.c
│
├── security/
│   └── bootloader.c
│
├── include/
│   └── xnu++/
│       └── bootloader.h
│
├── build/
│
└── build64/

The exact structure may evolve as additional platform components are introduced.

---

Development Model

xnu++ is being developed incrementally.

The development strategy is:

Boot
 ↓
CPU initialization
 ↓
Console
 ↓
Memory
 ↓
Interrupts
 ↓
Platform interfaces
 ↓
Device layer
 ↓
Process model
 ↓
Userspace
 ↓
Distribution
 ↓
Desktop

Each stage is intended to provide a usable foundation for the next stage.

The project avoids claiming that planned components already exist.

---

Current Implementation Status

Implemented / Working

The current project includes working development implementations for:

- xnu++ i386 ELF generation
- xnu++ x86-64 ELF generation
- Multiboot2 kernel headers
- i386 boot entry
- x86-64 long-mode boot entry
- x86-64 paging bring-up
- VGA console
- early terminal output
- basic GDT initialization
- IDT infrastructure
- PIC infrastructure
- PS/2 keyboard support
- basic memory allocator
- kernel memory tests
- kernel panic infrastructure
- provider architecture
- boot capability definitions
- boot policy definitions
- boot image structures
- recovery interface
- rollback interface
- security verification interfaces
- Orange OS i386 init build
- Orange OS x86-64 init build
- Orange OS root filesystem layout
- Orange OS CLI
- Orange OS configuration
- Orange OS package-management prototype
- Orange OS service-management prototype
- Orange OS ISO development workflow
- QEMU-based development and testing

---

Experimental Components

The following components are currently experimental and should not be considered production-ready:

- x86-64 interrupt handling
- native provider implementation
- XNU provider integration
- cryptographic boot verification
- SHA-256 measurement implementation
- rollback enforcement
- recovery implementation
- device abstraction layer
- process model
- userspace handoff
- package dependency resolution
- service supervision
- filesystem implementation
- networking
- KDE Plasma integration

---

Planned Components

Major planned areas include:

Kernel

- physical memory manager
- virtual memory manager
- page allocator
- scheduler
- process model
- threads
- address spaces
- syscall interface
- synchronization primitives
- SMP
- APIC
- timers
- kernel modules

Device Layer

- PCI
- PCIe
- USB
- storage
- networking
- input
- display
- audio
- device discovery

Filesystem

- VFS
- filesystem abstraction
- persistent storage
- permissions
- mount system
- filesystem drivers

Security

- real cryptographic verification
- secure boot integration
- measured boot
- key management
- capability enforcement
- sandboxing
- isolation
- security policies

Distribution

- native init system
- package manager
- dependency resolver
- package repositories
- package signatures
- service supervisor
- users
- permissions
- networking configuration
- system update mechanism

Desktop

- Wayland integration
- KDE Plasma packaging
- graphics stack
- display management
- desktop services

---

Design Principles

1. Separation of layers

Kernel code should not contain distribution-specific desktop logic.

xnu++ ≠ KDE
xnu++ ≠ Orange OS
Orange OS ≠ kernel

Each layer has a separate responsibility.

---

2. Incremental implementation

A component should not be described as complete simply because an interface exists.

For example:

Security API exists
        ≠
Production secure boot exists

Likewise:

KDE configuration exists
        ≠
KDE is fully integrated

This distinction is important for accurate project status.

---

3. Provider neutrality

Platform interfaces should allow multiple implementations where practical.

This makes it possible to evolve from existing XNU components toward native xnu++ implementations.

---

4. Explicit status

Components should be categorized as:

Implemented
Adapted
Experimental
Planned

This keeps project documentation technically honest.

---

5. Architecture independence

The project maintains i386 and x86-64 development paths.

The existence of a newer architecture should not automatically remove an older supported development target.

---

Limitations

xnu++ is an experimental operating-system engineering project.

It should not currently be considered a production operating system.

Important limitations include:

- incomplete hardware support
- incomplete process management
- incomplete userspace support
- incomplete filesystem support
- incomplete networking
- incomplete x86-64 interrupt architecture
- incomplete SMP support
- placeholder security implementations
- incomplete cryptographic verification
- incomplete recovery system
- incomplete package system
- incomplete desktop integration

The project is currently focused on building the architectural foundation.

---

Security Status

Security is a major architectural goal, but security interfaces must not be confused with complete security implementations.

The project currently contains interfaces for:

Signature verification
Measurement
Rollback protection
Capability requirements
Recovery

However, some implementations are still placeholders used for virtual-machine bring-up.

Therefore:

«Do not treat the current development builds as secure-boot-certified or production-secure systems.»

Real cryptographic implementations, key management, trusted boot roots, persistent rollback counters, and hardware-backed security mechanisms remain future work.

---

Roadmap

Phase 1 — Platform Bring-up

- [x] i386 boot
- [x] x86-64 boot
- [x] Multiboot2
- [x] VGA console
- [x] GDT
- [x] IDT foundation
- [x] PIC foundation
- [x] basic keyboard support
- [x] basic allocator
- [x] QEMU testing

Phase 2 — Platform Architecture

- [x] provider abstraction
- [x] boot capability model
- [x] boot image API
- [x] recovery interface
- [x] rollback interface
- [ ] native device abstraction
- [ ] process model
- [ ] scheduler
- [ ] virtual memory
- [ ] SMP

Phase 3 — Orange OS

- [x] root filesystem
- [x] Orange CLI
- [x] configuration
- [x] init prototype
- [x] package-management prototype
- [x] service-management prototype
- [x] i386 userspace build
- [x] x86-64 userspace build
- [x] development ISO
- [ ] native init
- [ ] native package manager
- [ ] dependency resolution
- [ ] persistent service supervisor
- [ ] filesystem integration

Phase 4 — Hardware

- [ ] PCI
- [ ] storage
- [ ] USB
- [ ] networking
- [ ] graphics
- [ ] input
- [ ] audio

Phase 5 — Security

- [ ] real signature verification
- [ ] real SHA-256
- [ ] trusted keys
- [ ] measured boot
- [ ] persistent rollback protection
- [ ] recovery environment
- [ ] security policy enforcement

Phase 6 — Desktop

- [ ] Wayland stack
- [ ] KDE Plasma packages
- [ ] display management
- [ ] graphics acceleration
- [ ] desktop services

---

Contributing

Contributions should preserve the layered architecture of the project.

When adding a component, clearly identify whether it belongs to:

xnu++ platform
kernel
provider
device layer
system service
Orange OS
userspace
desktop

Avoid moving distribution-specific functionality into the kernel unless there is a clear architectural reason.

Experimental code should be clearly documented as experimental.

Security-sensitive code should not use placeholder cryptography in production paths.

---

Development Philosophy

xnu++ is intended to be built from the lowest layers upward.

The project therefore values:

- direct hardware understanding
- explicit interfaces
- small components
- reproducible builds
- architecture separation
- clear security boundaries
- documented limitations
- incremental development
- practical testing

The goal is not simply to produce a bootable ELF.

The goal is to build a complete platform that can eventually support a complete operating-system distribution.

---

Relationship to XNU

xnu++ is architecturally inspired by and built around concepts associated with XNU, Mach, BSD, and IOKit.

The project should distinguish between:

Using an existing technology

and:

Implementing an independent subsystem

Where an existing XNU component is used or adapted, the project should document that relationship explicitly.

Where xnu++ implements its own subsystem, that subsystem should have its own interface and implementation.

This allows the project to evolve toward increasing independence without pretending that every subsystem is already independently implemented.

---

Relationship to Orange OS

Orange OS demonstrates how xnu++ can serve as a platform for a complete distribution.

The intended long-term stack is:

┌─────────────────────────────┐
│          Applications       │
├─────────────────────────────┤
│        KDE / CLI / Apps     │
├─────────────────────────────┤
│          Orange OS         │
│  init / packages / services │
├─────────────────────────────┤
│            xnu++            │
│   kernel / platform layer   │
├─────────────────────────────┤
│ XNU / Mach / BSD / IOKit    │
├─────────────────────────────┤
│          Hardware           │
└─────────────────────────────┘

This separation allows Orange OS to evolve independently as a distribution while xnu++ continues to evolve as a platform.

---

Current Development Target

The immediate development target is to complete the path:

Limine
   ↓
Multiboot2
   ↓
xnu++ x86-64
   ↓
Platform initialization
   ↓
Orange OS init
   ↓
Orange userspace
   ↓
System services
   ↓
Applications

Once the userspace handoff is stable, development can move toward filesystem support, process management, package management, networking, and desktop integration.

---

Project Status

xnu++ is an active experimental project.

The current stage demonstrates:

Bootable platform
        +
i386 support
        +
x86-64 support
        +
Multiboot2
        +
Low-level kernel infrastructure
        +
Security architecture
        +
Provider architecture
        +
Distribution architecture
        +
Orange OS userspace

The project is still under active development and many subsystems remain incomplete.

The purpose of the current implementation is to establish the foundation on which those subsystems can be built.

---

Summary

xnu++ provides the platform.

Orange OS provides the distribution.

The architecture is intentionally layered:

Hardware
   ↓
XNU / Mach / BSD / IOKit
   ↓
xnu++
   ↓
Orange OS
   ↓
Userspace
   ↓
Applications
   ↓
KDE Plasma (optional)

The project currently has working development paths for both i386 and x86-64, a Multiboot2-based boot path, low-level kernel infrastructure, an experimental security/boot architecture, and an initial Orange OS distribution environment.

The long-term objective is to evolve these foundations into a complete, modular, maintainable operating-system platform and a distribution built on top of it.
Distribution link   orange os:
https://github.com/black-210/orange-os
