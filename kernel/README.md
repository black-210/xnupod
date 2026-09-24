# xnu++ Kernel Layer

xnu++ uses a provider architecture.

Primary provider:

- XNU
- Mach
- BSD
- IOKit

Independent bring-up provider:

- native xnu++ kernel layer

The native provider is not a replacement for XNU.
It exists for independent kernel bring-up and development.

Orange OS is a userspace/distribution project built above xnu++.
