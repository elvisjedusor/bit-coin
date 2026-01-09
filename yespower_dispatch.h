// Runtime CPU feature detection and dispatch for yespower
// Auto-detects AVX2, SSE4.1, SSE2 and uses best available implementation

#ifndef YESPOWER_DISPATCH_H
#define YESPOWER_DISPATCH_H

#include "yespower.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// CPU feature flags
#define CPU_FEATURE_SSE2   (1 << 0)
#define CPU_FEATURE_SSE41  (1 << 1)
#define CPU_FEATURE_AVX    (1 << 2)
#define CPU_FEATURE_AVX2   (1 << 3)
#define CPU_FEATURE_XOP    (1 << 4)

// Runtime CPU feature detection
uint32_t detect_cpu_features(void);
const char* get_cpu_name(void);

// Initialize yespower with best available CPU optimization
void yespower_init_dispatch(void);

#ifdef __cplusplus
}
#endif

#endif // YESPOWER_DISPATCH_H
