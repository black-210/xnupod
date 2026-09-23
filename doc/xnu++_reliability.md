# xnu++ semi-independent reliability architecture

xnu++ is **semi-independent**: it reuses mature XNU interfaces and components where they are reliable, while providing provider-neutral services for portability, recovery, testing, and future implementations. Independence is a means to reliability, not an all-or-nothing rewrite.

## Reliability principles

- Prefer a proven provider for a capability when it passes health checks.
- Never silently switch providers during a security-sensitive operation.
- Permit controlled failover for non-security-critical services.
- Keep recovery, diagnostics, policy, and validation outside Apple-only infrastructure.
- Version xnu++ APIs independently from XNU internals.
- Make unsupported hardware explicit and safe.

## Feature areas beyond the imported baseline

The xnu++ layer is intended to add capabilities that are not consistently available in upstream XNU distributions:

- provider health monitoring and bounded failover
- capability negotiation instead of platform-name assumptions
- checkpoint-assisted service recovery
- signed, transactional updates with rollback
- resource quotas and service isolation
- reproducible device/support reports
- host-side simulation for buses and drivers
- structured crash and reliability telemetry
- deterministic test profiles for virtual machines
- graceful degradation when optional accelerators or firmware are unavailable

These are interfaces and engineering goals until an implementation has tests and a documented support matrix. xnu++ must not claim that a feature exists merely because an API has been declared.
