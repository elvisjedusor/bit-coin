// Runtime CPU feature detection and dispatch for yespower
// Auto-detects AVX2, SSE4.1, SSE2 and uses best available implementation

#include "yespower_dispatch.h"
#include <stdio.h>
#include <string.h>

#if defined(__x86_64__) || defined(__i386__) || defined(_M_X64) || defined(_M_IX86)
#define HAVE_CPUID 1

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <cpuid.h>
#endif

static uint32_t g_cpu_features = 0;
static char g_cpu_brand[64] = {0};
static int g_initialized = 0;

// CPUID helper
static void cpuid(uint32_t leaf, uint32_t subleaf, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
#ifdef _MSC_VER
    int regs[4];
    __cpuidex(regs, leaf, subleaf);
    *eax = regs[0];
    *ebx = regs[1];
    *ecx = regs[2];
    *edx = regs[3];
#else
    __cpuid_count(leaf, subleaf, *eax, *ebx, *ecx, *edx);
#endif
}

// Detect CPU features
uint32_t detect_cpu_features(void)
{
    uint32_t eax, ebx, ecx, edx;
    uint32_t features = 0;

    // Check if CPUID is supported
    cpuid(0, 0, &eax, &ebx, &ecx, &edx);
    uint32_t max_leaf = eax;

    // Get CPU vendor
    if (max_leaf >= 0) {
        uint32_t vendor[4] = {ebx, edx, ecx, 0};
        memcpy(g_cpu_brand, vendor, 12);
        g_cpu_brand[12] = '\0';
    }

    // Get CPU brand string (extended)
    cpuid(0x80000000, 0, &eax, &ebx, &ecx, &edx);
    if (eax >= 0x80000004) {
        uint32_t* brand_ptr = (uint32_t*)g_cpu_brand;
        cpuid(0x80000002, 0, &brand_ptr[0], &brand_ptr[1], &brand_ptr[2], &brand_ptr[3]);
        cpuid(0x80000003, 0, &brand_ptr[4], &brand_ptr[5], &brand_ptr[6], &brand_ptr[7]);
        cpuid(0x80000004, 0, &brand_ptr[8], &brand_ptr[9], &brand_ptr[10], &brand_ptr[11]);
        g_cpu_brand[47] = '\0';

        // Trim leading spaces
        char* brand = g_cpu_brand;
        while (*brand == ' ') brand++;
        if (brand != g_cpu_brand) {
            memmove(g_cpu_brand, brand, strlen(brand) + 1);
        }
    }

    // Check basic CPU features (leaf 1)
    if (max_leaf >= 1) {
        cpuid(1, 0, &eax, &ebx, &ecx, &edx);

        // EDX bits
        if (edx & (1 << 26)) features |= CPU_FEATURE_SSE2;   // SSE2

        // ECX bits
        if (ecx & (1 << 19)) features |= CPU_FEATURE_SSE41;  // SSE4.1
        if (ecx & (1 << 28)) features |= CPU_FEATURE_AVX;    // AVX
    }

    // Check extended features (leaf 7)
    if (max_leaf >= 7) {
        cpuid(7, 0, &eax, &ebx, &ecx, &edx);

        // EBX bits
        if (ebx & (1 << 5)) {
            // AVX2 requires both AVX and AVX2 bit
            if (features & CPU_FEATURE_AVX) {
                features |= CPU_FEATURE_AVX2;
            }
        }
    }

    // Check AMD extended features for XOP
    cpuid(0x80000000, 0, &eax, &ebx, &ecx, &edx);
    if (eax >= 0x80000001) {
        cpuid(0x80000001, 0, &eax, &ebx, &ecx, &edx);
        if (ecx & (1 << 11)) features |= CPU_FEATURE_XOP;    // XOP (AMD)
    }

    return features;
}

const char* get_cpu_name(void)
{
    if (!g_initialized) {
        yespower_init_dispatch();
    }
    return g_cpu_brand[0] ? g_cpu_brand : "Unknown CPU";
}

void yespower_init_dispatch(void)
{
    if (g_initialized) return;

    g_cpu_features = detect_cpu_features();
    g_initialized = 1;

    // Print detected features
    printf("CPU: %s\n", get_cpu_name());
    printf("Yespower optimizations: ");

    if (g_cpu_features & CPU_FEATURE_XOP) {
        printf("XOP + ");
    }
    if (g_cpu_features & CPU_FEATURE_AVX2) {
        printf("AVX2 + ");
    } else if (g_cpu_features & CPU_FEATURE_AVX) {
        printf("AVX + ");
    }
    if (g_cpu_features & CPU_FEATURE_SSE41) {
        printf("SSE4.1 + ");
    }
    if (g_cpu_features & CPU_FEATURE_SSE2) {
        printf("SSE2");
    } else {
        printf("GENERIC (slow!)");
    }
    printf("\n");
}

#else
// Non-x86 platforms - no runtime detection
uint32_t detect_cpu_features(void) { return 0; }
const char* get_cpu_name(void) { return "Non-x86 CPU"; }
void yespower_init_dispatch(void) {
    printf("Yespower: Generic implementation (non-x86)\n");
}
#endif
