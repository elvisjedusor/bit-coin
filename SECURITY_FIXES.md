# Bitok Security Hardening & Network Improvements (v0.3.19.8)

## Overview

This document describes security improvements implemented in Bitok. All fixes are **backwards-compatible** - no consensus hard fork required.

| Issue | Severity | Status | Consensus Impact |
|-------|----------|--------|------------------|
| Time Warp Attack | CRITICAL | IMPLEMENTED | Soft-fork (activates at block 16000) |
| No DNS Seeds | HIGH | IMPLEMENTED | None (network layer) |
| External HTTP for IP | MEDIUM | IMPLEMENTED | None (network layer) |

---

## Issue 1: Time Warp Attack Vulnerability

### Problem

The difficulty adjustment algorithm uses an "off-by-one" loop pattern inherited from original Bitcoin. While this is technically correct behavior, the lack of timestamp validation at difficulty boundaries could allow miners to manipulate timestamps.

### Solution Implemented

File: `main.cpp`, function `CBlock::AcceptBlock()` (lines 1472-1511)

```cpp
// Time warp attack mitigation (soft-fork)
// Activates at block TIMEWARP_ACTIVATION_HEIGHT
// Before activation: warnings only
// After activation: strict enforcement (rejects invalid blocks)
const unsigned int nTargetSpacing = 10 * 60;
const unsigned int nMaxTimestampDrift = 7200;
int nNextHeight = pindexPrev->nHeight + 1;
bool fEnforceTimewarp = (nNextHeight >= TIMEWARP_ACTIVATION_HEIGHT);

if (nTime > pindexPrev->nTime + nMaxTimestampDrift + nTargetSpacing * 6)
{
    printf("WARNING: Block timestamp %u is far ahead of parent %u (diff=%u)\n",
           nTime, pindexPrev->nTime, nTime - pindexPrev->nTime);
    if (fEnforceTimewarp)
        return error("AcceptBlock() : block timestamp too far ahead (time warp protection)");
}

// At difficulty adjustment boundaries, apply stricter validation
const unsigned int nTargetTimespan = 14 * 24 * 60 * 60;
const unsigned int nInterval = nTargetTimespan / nTargetSpacing;
if (nNextHeight % nInterval == 0)
{
    const CBlockIndex* pindexFirst = pindexPrev;
    for (int i = 0; pindexFirst && i < (int)(nInterval - 1); i++)
        pindexFirst = pindexFirst->pprev;

    if (pindexFirst)
    {
        int64 nActualTimespan = (int64)nTime - (int64)pindexFirst->nTime;
        int64 nMinReasonableTimespan = (int64)nTargetTimespan / 8;

        if (nActualTimespan < nMinReasonableTimespan)
        {
            printf("WARNING: Difficulty adjustment timespan %lld is suspiciously short (min=%lld)\n",
                   nActualTimespan, nMinReasonableTimespan);
            if (fEnforceTimewarp)
                return error("AcceptBlock() : timespan too short (time warp protection)");
        }
    }
}
```

### Behavior

**Before Block 16000:**
- Logs warnings for suspicious timestamps
- Does NOT reject blocks (full backwards compatibility)
- Allows monitoring of potential attacks

**After Block 16000 (Mandatory Activation):**
- Time warp protection enforced automatically for all nodes
- Blocks with suspicious timestamps are rejected
- No configuration required - protection is built-in

---

## Issue 2: DNS Seeds for Network Bootstrap

### Problem

Only hardcoded IP addresses for network bootstrapping - insufficient for reliable peer discovery.

### Solution Implemented

File: `net.cpp`, lines 869-914

```cpp
// Bitok: DNS seeds for reliable network bootstrap
static const char* pszDNSSeeds[] = {
    "seed1.bitokd.run",
    "seed2.bitokd.run",
    "seed3.bitokd.run",
    NULL
};

void QueryDNSSeeds()
{
    printf("[DNS] Querying DNS seeds for peer discovery\n");

    int nAddresses = 0;
    for (int i = 0; pszDNSSeeds[i] != NULL; i++)
    {
        if (fShutdown)
            return;

        printf("[DNS] Resolving: %s\n", pszDNSSeeds[i]);

        struct hostent* phostent = gethostbyname(pszDNSSeeds[i]);
        if (!phostent || !phostent->h_addr_list)
        {
            printf("[DNS] Failed to resolve %s\n", pszDNSSeeds[i]);
            continue;
        }

        for (int j = 0; phostent->h_addr_list[j] != NULL; j++)
        {
            CAddress addr;
            addr.ip = *(unsigned int*)phostent->h_addr_list[j];
            addr.port = DEFAULT_PORT;
            addr.nServices = NODE_NETWORK;
            addr.nTime = GetAdjustedTime() - 3 * 24 * 60 * 60;

            if (addr.IsValid() && addr.IsRoutable() && addr.ip != addrLocalHost.ip)
            {
                printf("[DNS] Got peer: %s\n", addr.ToString().c_str());
                AddAddress(addr);
                nAddresses++;
            }
        }
    }

    printf("[DNS] Discovered %d addresses from DNS seeds\n", nAddresses);
}
```

Called in `ThreadOpenConnections2()` during network startup (line 1010).

### DNS Infrastructure Required

Set up DNS A records for:
- `seed1.bitokd.run` - pointing to reliable nodes
- `seed2.bitokd.run` - pointing to reliable nodes
- `seed3.bitokd.run` - pointing to reliable nodes

Consider running [bitcoin-seeder](https://github.com/sipa/bitcoin-seeder) for dynamic DNS seed management.

### Fallback

Hardcoded seeds exist as fallback (lines 916-923):
```cpp
unsigned int pnSeed[] =
{
    0x215D6F40, // 64.111.93.33
    0x0A9F0DC6, // 198.13.159.10
    0x36C6FCA2, // 162.252.198.54
    0xCF62D9C7, // 199.217.98.207
};
```

---

## Issue 3: External HTTP Dependencies Removed

### Problem

Used HTTP (not HTTPS) connections to external services for IP detection - privacy risk and centralized dependency.

### Solution Implemented

File: `net.cpp`, lines 239-257

```cpp
bool GetMyExternalIP(unsigned int& ipRet)
{
    if (fUseProxy)
        return false;

    if (mapArgs.count("-externalip"))
    {
        CAddress addr(mapArgs["-externalip"].c_str());
        if (addr.IsValid())
        {
            printf("GetMyExternalIP() using configured -externalip: %s\n", addr.ToString().c_str());
            ipRet = addr.ip;
            return true;
        }
    }

    printf("GetMyExternalIP() skipped - external IP will be learned from peers\n");
    return false;
}
```

### Behavior

- **Removed all HTTP calls to external services**
- External IP is now learned from peers via VERSION messages (standard P2P protocol)
- Manual override available via `-externalip=X.X.X.X` flag

### Configuration

For nodes behind NAT that need to advertise specific IP:
```
externalip=1.2.3.4
```

---

## Consensus Compatibility

**All fixes maintain full backwards compatibility:**

1. **Time Warp Fix**:
   - Before block 16000: Warnings only (no consensus change)
   - After block 16000: Mandatory enforcement (soft-fork activation)

2. **DNS Seeds**: Network layer only - no consensus impact

3. **External IP**: Network layer only - no consensus impact

**Existing blocks, transactions, and chain history remain fully valid. No hard fork required.**

---

## Recommended Node Configuration

```ini
# bitok.conf

# Manual external IP if behind NAT
# externalip=YOUR_PUBLIC_IP

# Connect to additional trusted nodes
addnode=seed1.bitokd.run
addnode=seed2.bitokd.run

# Enable debug logging for monitoring
debug=1
```