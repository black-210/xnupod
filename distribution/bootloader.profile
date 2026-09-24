# xnu++ bootloader policy profile
# This profile describes required behavior for a future boot manager.
schema = 1
provider = xnu
architecture = x86_64,arm64
firmware = uefi,apple-firmware,provider-defined
entry_manifest = /System/Library/Boot/Entries/boot.manifest
trust_anchor = provider-defined
secure_boot = required
measured_boot = required
signature_algorithm = provider-defined
rollback_protection = required
boot_attempt_limit = 3
recovery = signed-offline
serial_console = required
text_ui = required
graphical_ui = optional
required_artifacts = kernel,boot-args,hardware-description
optional_artifacts = drivers,initrd,ui-assets
unknown_artifacts = reject
unknown_required_capabilities = reject
provider_failover = forbidden
