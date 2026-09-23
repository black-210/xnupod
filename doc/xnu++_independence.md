# xnu++ independence boundary

xnu++ is becoming feature-independent while remaining implementation-compatible with XNU. This distinction is deliberate:

- **Feature-independent:** update policy, recovery, quotas, service isolation, capability reporting, diagnostics, and device policy do not require Apple-private APIs.
- **Provider-compatible:** XNU can implement the contracts using Mach, BSD, and IOKit where those components are reliable.
- **Replaceable:** a future native provider can implement the same contracts without reproducing every XNU internal.
- **Honest:** hardware, firmware, boot, cryptography, and performance support are marked unsupported until tested.

The independence boundary is a reliability feature. It permits xnu++ to preserve mature kernel behavior while replacing neglected policy and distribution plumbing. It does not claim that the project already contains a complete independent kernel, bootloader, driver ecosystem, or distribution.

## Required invariants

1. Security failures never trigger an untrusted provider failover.
2. Updates are verified before commit and have a bounded rollback path.
3. Resource exhaustion is contained to the responsible subject where possible.
4. Unknown devices and undeclared privileged access are denied by default.
5. Diagnostics must not expose secrets or permit control-plane mutation.
