# Bitok Proof-of-Work: Yespower

CPU-optimized, GPU/ASIC-resistant mining as Satoshi intended.

---

## Overview

Bitok replaces Bitcoin's SHA-256 proof-of-work with Yespower 1.0, a memory-hard, CPU-optimized hashing algorithm designed to resist GPU and ASIC acceleration.

This implements what Satoshi Nakamoto requested but never received: a mining algorithm that keeps mining accessible to ordinary users with regular computers.

### What Changed

Bitcoin (original SHA-256):
```cpp
uint256 hash = block.GetHash();  // Double SHA-256
if (hash <= target) { /* block found */ }
```

Bitok:
```cpp
uint256 hash = YespowerHashBlock(&block, 80);  // Yespower
if (hash <= target) { /* block found */ }
```

The entire change is swapping one hash function for another. Everything else — difficulty adjustment, block validation, consensus rules — remains exactly as Satoshi wrote it in Bitcoin v0.3.19.

---

## Why Yespower

### Satoshi's Requirement

On December 12, 2010, Satoshi wrote:

> We should have a gentleman's agreement to postpone the GPU arms race as long as we can for the good of the network.

And in correspondence with Laszlo Hanyecz:

> It's inevitable that GPU compute clusters will eventually hog all the generated coins, but I don't want to hasten that day.

Bitcoin failed this requirement. SHA-256 is perfectly parallelizable and was quickly dominated by GPUs (100x faster than CPU), then ASICs (1000x faster than GPU), then industrial mining farms.

Bitok enforces this requirement. Yespower is designed to be memory-hard (requires ~128KB RAM per hash), CPU-optimized (uses SIMD instructions that GPUs don't have), and ASIC-resistant (memory access patterns make ASIC design uneconomical).

### Design Goals

| Goal | SHA-256 | Yespower |
|------|---------|----------|
| CPU-friendly | no | yes |
| GPU-resistant | no | yes |
| ASIC-resistant | no | yes |
| Laptop mining viable | no | yes |
| Memory-hard | no | yes |
| Simple verification | yes | yes |
| Well-studied | yes | yes |

---

## Technical Specifications

### Bitok Parameters

```c
static const yespower_params_t yespower_params = {
    .version = YESPOWER_1_0,
    .N = 2048,                              // memory cost parameter
    .r = 32,                                // block size parameter
    .pers = (const uint8_t*)"BitokPoW",     // personalization string
    .perslen = 8                            // length of personalization
};
```

### Parameter Explanation

N (Memory Cost Parameter) = 2048. Determines memory usage: ~128KB per hash (N × r × 128 bytes). Large enough to resist GPU parallelization. Small enough to run efficiently on any modern CPU.

r (Block Size Parameter) = 32. Controls mixing complexity and memory access patterns. Optimal balance between security and performance.

Personalization String = "BitokPoW". Domain separation from other Yespower implementations. Prevents hash reuse from other Yespower coins. Ensures Bitok hashes are unique to Bitok.

---

## Algorithm Details

### High-Level Flow

```
Block Header (80 bytes)
    ↓
Yespower Input Processing
    ↓
Memory Initialization (128KB)
    ↓
Sequential Memory-Hard Mixing
    ├─ ROMix iterations
    ├─ Random memory access
    └─ SIMD operations (AVX2/AVX512)
    ↓
Final Hash (256 bits)
    ↓
Compare to Target
```

### Yespower Algorithm

Yespower is based on scrypt but with significant improvements for CPU optimization and GPU resistance.

Step 1: Input Processing
```
Input: Block header (80 bytes) + Personalization ("BitokPoW")
Expand using PBKDF2-SHA256
```

Step 2: Memory Allocation
```
Allocate V[0..N-1] array (2048 × 64 bytes = 128KB)
Initialize V[0] from expanded input
```

Step 3: SMix (Sequential Memory-Hard Mixing)
```
for i = 0 to N-1:
    V[i] = BlockMix(V[i-1])         # Fill memory sequentially

for i = 0 to N-1:
    j = Integerify(X) mod N          # Generate pseudo-random index
    X = BlockMix(X XOR V[j])         # Random memory access
```

Step 4: Final Output
```
Apply PBKDF2 to mixed state
Return 256-bit hash
```

### Key Features

Sequential Memory Initialization. Must fill 128KB of memory before starting. Cannot be parallelized. Cache-friendly for CPUs, cache-hostile for GPUs.

Random Memory Access. Pseudo-random reads from the 128KB buffer. Memory latency becomes the bottleneck. GPUs have poor random memory access compared to CPUs.

SIMD Optimizations. Uses SSE2, AVX, AVX2, and AVX512 instructions. Automatically detected at runtime. Modern CPUs benefit greatly, GPUs don't have equivalent instructions.

Personalization. "BitokPoW" mixed into the algorithm. Prevents cross-chain hash reuse. Part of the hash input, not just metadata.

---

## Performance Characteristics

### CPU Mining Performance

Modern CPUs achieve 1-5 kH/s per core depending on SIMD support:

| CPU Feature | Hash Rate (per core) |
|-------------|---------------------|
| SSE2 only | ~1.0 kH/s |
| AVX | ~2.0 kH/s |
| AVX2 | ~3.5 kH/s |
| AVX512 | ~5.0 kH/s |

Example hardware:
- Intel i5-1135G7 (laptop, 4 cores): ~12 kH/s total
- AMD Ryzen 5 5600X (desktop, 6 cores): ~21 kH/s total
- Intel Xeon Gold 6248R (server, 48 cores): ~168 kH/s total

### GPU Mining Performance

GPUs are significantly slower at Yespower due to memory-hard design:

| Device | SHA-256 | Yespower | Ratio |
|--------|---------|----------|-------|
| Intel i7-10700K | 10 MH/s | 15 kH/s | baseline |
| NVIDIA RTX 3080 | 3000 MH/s | ~100 kH/s | 300x → 6x |
| AMD RX 6800 XT | 2500 MH/s | ~80 kH/s | 250x → 5x |

With SHA-256, GPUs are 300x faster than CPUs. With Yespower, GPUs are only 5-6x faster and consume far more power, making them uneconomical for mining.

### Why GPUs Can't Win

Memory Bandwidth. GPUs have high bandwidth but poor latency for random access. Yespower requires 128KB per hash with pseudo-random access patterns. GPU memory controllers are optimized for sequential access.

SIMD Instructions. Modern x86 CPUs have AVX2/AVX512 which Yespower exploits. GPUs have different SIMD architectures that don't map efficiently to Yespower's operations.

Power Efficiency. A CPU mining Yespower uses 50-100W and achieves reasonable hashrate. A GPU uses 250-350W for only 5-6x improvement. Cost per hash favors CPUs.

---

## Mining Configuration

### Enable Mining

```bash
# Start mining with all CPU cores
./bitokd -gen

# Limit to specific number of cores
./bitokd -gen -genproclimit=4
```

### RPC Control

```bash
# Enable mining with 4 threads
./bitokd setgenerate true 4

# Check mining status
./bitokd getgenerate

# Get network difficulty
./bitokd getdifficulty
```

### GUI Control

Settings → Options → Generate Coins

Check the box to enable. Adjust processor limit if desired.

---

## Existing Miner Compatibility

### Standalone Miners

Existing CPU miners like cpuminer-multi or cpuminer-opt support Yespower but require Bitok-specific parameters.

Example configuration for cpuminer:
```bash
./cpuminer \
    --algo=yespower \
    --param-n=2048 \
    --param-r=32 \
    --param-key="BitokPoW" \
    --url=http://127.0.0.1:8332 \
    --user=rpcuser \
    --pass=rpcpassword
```

Note: Check if your miner supports personalization strings. Not all Yespower miners support the `--param-key` option.

### Mining Pools

Pool operators can use standard stratum protocol with Yespower support. The personalization string "BitokPoW" must be configured in the pool software.

---

## Comparison to SHA-256

### Security Properties

Both SHA-256 and Yespower provide 256-bit security against preimage attacks. Both have been extensively studied.

SHA-256 has been analyzed since 2001. Yespower is based on scrypt (2009) with improvements from 2018.

### Mining Decentralization

SHA-256 led to mining centralization. First GPUs, then ASICs, then industrial farms. Today, Bitcoin mining requires specialized hardware and large capital investment.

Yespower maintains mining decentralization. Anyone with a laptop can mine. No special hardware needed. No driver configuration. No manufacturing oligopolies.

### Verification Speed

SHA-256 verification is extremely fast (~1 microsecond per block header).

Yespower verification is slower (~10 milliseconds per block header) but still fast enough for block propagation and validation.

For context, Bitcoin has 10-minute block times. A 10ms verification delay is negligible.

---

## Security Analysis

### Attack Resistance

Preimage Attacks. Finding a hash collision requires 2^256 operations. Both SHA-256 and Yespower are resistant.

GPU Mining Attacks. SHA-256 allows 100-1000x speedup with GPUs. Yespower reduces this to 5-6x while making GPUs uneconomical due to power costs.

ASIC Mining Attacks. SHA-256 ASICs provide 100,000x speedup over CPUs. Yespower's memory-hard design makes ASICs impractical (memory cost dominates chip design).

Memory-Hardness Attacks. Yespower requires 128KB per hash. Attempts to reduce memory usage result in exponentially slower performance. Time-memory tradeoff attacks are ineffective.

### Known Weaknesses

None that affect practical security. Yespower is based on well-studied cryptographic primitives (SHA-256, PBKDF2, scrypt).

The personalization string "BitokPoW" prevents hash reuse from other Yespower implementations.

---

## Implementation Details

### Code Integration

The Yespower implementation in Bitok consists of:

1. `yespower.h` - algorithm interface
2. `yespower.c` - core algorithm implementation
3. `yespower-opt.c` - SIMD optimizations (AVX/AVX2/AVX512)
4. `yespower_dispatch.c` - runtime CPU feature detection

Integration points in Bitcoin v0.3.19 code:

**main.cpp** - Block validation:
```cpp
// Old: uint256 hash = block.GetHash();
uint256 hash = YespowerHashBlock(&block, 80);
```

**main.cpp** - Mining loop:
```cpp
// Old: if (block.GetHash() <= hashTarget)
if (YespowerHashBlock(&block, 80) <= hashTarget)
```

No other changes required. Difficulty adjustment, block acceptance, and consensus rules remain identical.

### CPU Feature Detection

Yespower automatically detects and uses the best available CPU instructions:

```c
// Runtime detection (simplified)
if (cpu_has_avx512f()) {
    use_yespower_avx512();
} else if (cpu_has_avx2()) {
    use_yespower_avx2();
} else if (cpu_has_avx()) {
    use_yespower_avx();
} else {
    use_yespower_sse2();  // baseline
}
```

This ensures optimal performance on any CPU without manual configuration.

---

## FAQ

### Why not scrypt?

Scrypt is memory-hard but not optimized for modern CPUs. Yespower builds on scrypt with SIMD optimizations that significantly improve CPU performance while maintaining GPU resistance.

### Why not other algorithms?

Other options (Argon2, RandomX, etc.) were considered. Yespower was chosen for its proven track record in multiple cryptocurrencies, excellent CPU optimization, and strong GPU resistance.

### Can this be changed later?

No. The proof-of-work algorithm is part of the consensus rules. Changing it would require a hard fork and defeat the purpose of preserving Satoshi's design.

### What about quantum computers?

Yespower (like SHA-256) is vulnerable to Grover's algorithm, which provides quadratic speedup on quantum computers. This reduces security from 256 bits to 128 bits, which is still considered secure.

Mining centralization via quantum computers is a theoretical concern decades away. By the time quantum computers can mine efficiently, the entire cryptographic landscape will have changed.

---

## Conclusion

Yespower proof-of-work implements Satoshi's vision of accessible CPU mining. Anyone with a laptop can participate in securing the network without specialized hardware.

This is not innovation. This is restoration.

---

## References

- Yespower whitepaper: https://www.openwall.com/yespower/
- scrypt specification: https://tools.ietf.org/html/rfc7914
- Satoshi's GPU mining quotes: https://satoshi.nakamotoinstitute.org/quotes/mining/
- Bitcoin v0.3.19 source: https://github.com/bitcoin/bitcoin/tree/v0.3.19
