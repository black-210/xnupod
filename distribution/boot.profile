# xnu++ boot platform profile
schema = 1
boot_manager = required
boot_handoff = versioned
manifest = signed
kernel = signed
auxiliary_drivers = signed
measurements = required
rollback = required
recovery = signed-offline
required = uart,storage,console
optional = framebuffer,uefi-graphics,network-recovery
ui = text-first
kde = optional-userspace

# A profile is accepted only when every item in `required` is tested for the
# selected provider. Optional capabilities must be reported, never guessed.
