#ifndef XNUXX_DIAGNOSTICS_H
#define XNUXX_DIAGNOSTICS_H

#include <stddef.h>
#include <stdint.h>

#define XNUXX_DIAGNOSTICS_API_VERSION 1u

enum xnuxx_diagnostic_severity { XNUXX_DIAG_INFO = 1, XNUXX_DIAG_WARNING = 2, XNUXX_DIAG_ERROR = 3, XNUXX_DIAG_FATAL = 4 };
struct xnuxx_diagnostic { uint32_t api_version; enum xnuxx_diagnostic_severity severity; uint64_t timestamp; uint64_t provider_generation; const char *component; const char *message; };
typedef int (*xnuxx_diagnostic_sink)(const struct xnuxx_diagnostic *, void *);
int xnuxx_diagnostics_subscribe(xnuxx_diagnostic_sink, void *);
int xnuxx_diagnostics_emit(const struct xnuxx_diagnostic *);
int xnuxx_diagnostics_unsubscribe(xnuxx_diagnostic_sink, void *);

#endif
