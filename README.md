# Bitok - Bitcoin v0.3.0 Time Machine

Pure Satoshi code. Adapted to modern systems. CPU mining restored.

Copyright (c) 2009-2010 Satoshi Nakamoto

Copyright (c) 2026 Tom Elvis Jedusor

Distributed under the MIT/X11 software license

---

## Documentation Table of Contents

### Quick Start
- [What Is This](#what-is-this)
- [Building](#building)
- [Running](#running)
- [Mining](#mining)

### Technical Documentation
- [RPC API Reference](RPC_API.md) - complete JSON-RPC API for exchanges, mining pools, block explorers
- [Yespower Mining Algorithm](BITOKPOW.md) - technical details on CPU-optimized proof-of-work
- [Project Manifesto](MANIFESTO.md) - philosophy and purpose

### Build Guides
- [Unix/Linux Build Guide](BUILD_UNIX.md)
- [macOS Build Guide](BUILD_MACOS.md)
- [Windows Build Guide](BUILD_WINDOWS.md)

### Legal & Resources
- [License](license.txt) - MIT/X11 License
- [Links](#links)

---

## What Is This

Bitok is Bitcoin version 0.3.0, the last version Satoshi Nakamoto personally released, resurrected to run on modern operating systems.

This is not a fork in the political sense. This is not BTC with changes. This is Satoshi's original code from 2010, with only three categories of modifications:

1. System compatibility - updated to compile and run with modern toolchains
2. GPU-resistant mining - SHA-256 replaced with Yespower to implement Satoshi's vision of CPU-only mining
3. Separate network - new genesis block to avoid interfering with BTC

Everything else is untouched. Same consensus rules. Same economics. Same peer-to-peer behavior. Same philosophy.

## Satoshi on GPU Mining

On December 12, 2010, Satoshi Nakamoto wrote on BitcoinTalk:

> We should have a gentleman's agreement to postpone the GPU arms race as long as we can for the good of the network. It's much easer to get new users up to speed if they don't have to worry about GPU drivers and compatibility. It's nice how anyone with just a CPU can compete fairly equally right now.

In private correspondence with early Bitcoin developer Laszlo Hanyecz, Satoshi explained why GPU mining was a problem:

> GPUs are much less evenly distributed, so the generated coins only go towards rewarding 20% of the people for joining the network instead of 100%. I don't mean to sound like a socialist, I don't care if wealth is concentrated, but for now, we get more growth by giving that money to 100% of the people than giving it to 20%.

He knew what was coming:

> It's inevitable that GPU compute clusters will eventually hog all the generated coins, but I don't want to hasten that day.

Bitcoin ignored this. Bitok enforces it.

## What Changed From Bitcoin v0.3.0

### Code Changes Summary

| Category | Changes | Purpose |
|----------|---------|---------|
| System Compatibility | OpenSSL 3.x, Berkeley DB 5.3 C API, Boost 1.74+, GCC 11+, wxWidgets 3.2 | compile on modern Ubuntu 24.04 |
| Proof-of-Work | SHA-256 → Yespower 1.0 (N=2048, r=32) | CPU-friendly, GPU/ASIC-resistant |
| Network | new genesis block | separate network from BTC |

No features. No protocol changes. No layers. No "improvements."

See [BITOKPOW.md](BITOKPOW.md) for technical details on the Yespower proof-of-work algorithm.

## Building

### Dependencies (Ubuntu 24.04)

```bash
sudo apt-get update
sudo apt-get install build-essential libssl-dev libdb-dev libdb5.3-dev libboost-all-dev

# For GUI (optional)
sudo apt-get install libwxgtk3.2-dev libgtk-3-dev
```

### Compile

```bash
# Daemon only
make -f makefile.unix

# GUI wallet
make -f makefile.unix gui

# Both
make -f makefile.unix all
```

See [BUILD_UNIX.md](BUILD_UNIX.md) for detailed build instructions for all platforms.

## Running

### Daemon

```bash
./bitokd                    # start node
./bitokd -gen               # start node with mining enabled
./bitokd -daemon -gen       # run in background with mining
./bitokd getinfo            # get node info
./bitokd help               # list all commands
```

### GUI

```bash
./bitok                     # launch graphical wallet
```

The GUI provides a user-friendly interface for:
- sending and receiving coins
- viewing transaction history
- generating new addresses
- controlling mining

## Mining

Mining on Bitok works exactly as it did in Bitcoin v0.3.0, except it uses Yespower instead of SHA-256.

Enable mining:
```bash
./bitokd -gen                           # start mining on all CPU cores
./bitokd -gen -genproclimit=4           # limit to 4 cores
```

In the GUI: Settings → Options → Generate Coins

Why Yespower:
- memory-hard algorithm (requires ~128KB RAM per hash)
- CPU-optimized with automatic SIMD detection (SSE2/AVX/AVX2/AVX512)
- GPU/ASIC-resistant by design
- maintains Satoshi's vision: anyone with a laptop can mine

See [BITOKPOW.md](BITOKPOW.md) for technical details on the mining algorithm.

## Philosophy

Read [MANIFESTO.md](MANIFESTO.md) for the full context.

Short version:

Bitcoin was meant to be peer-to-peer electronic cash. Mining was meant to be accessible to everyone. The protocol was meant to be simple and unchanging. Privacy was meant to be natural, not bolted on. There were no leaders, no roadmaps, no foundations.

Bitcoin v0.3.0 embodied these principles. Modern Bitcoin abandoned them.

Bitok restores them.

## What This Is Not

This is not BTC. Different network, different genesis, different mining algorithm.

This is not a proposal. No one needs to agree. Run it or don't.

This is not a political fork. No debate, no governance, no ideology injection.

This is not trying to be Bitcoin. Bitcoin already exists and has made its choices.

This is not promising anything. No roadmap, no adoption guarantees, no price targets.

This is software that runs. If you run it, you are the network. If you don't, you aren't.

## Security Notice

This is code from 2010, adapted for modern systems.

The same security model as Bitcoin v0.3.0. Modern cryptography (OpenSSL 3.x). GPU-resistant mining (Yespower). No modern "features" (good). No peer review since 2010 (expected). No guarantees (as intended).

You are responsible for your keys. You are responsible for your node. You are responsible for your mistakes.

## Documentation

### For Developers & Integrators
- [RPC_API.md](RPC_API.md) - complete JSON-RPC API reference for exchanges, mining pools, and block explorers

### Philosophy & Technical Details
- [MANIFESTO.md](MANIFESTO.md) - philosophy and purpose
- [BITOKPOW.md](BITOKPOW.md) - technical details on Yespower mining algorithm

### Build Instructions
- [BUILD_UNIX.md](BUILD_UNIX.md) - Unix/Linux build instructions
- [BUILD_MACOS.md](BUILD_MACOS.md) - macOS build instructions
- [BUILD_WINDOWS.md](BUILD_WINDOWS.md) - Windows build instructions

## License

MIT/X11 License - See [license.txt](license.txt)

## Authors

Satoshi Nakamoto - original Bitcoin v0.3.0 (2009-2010)
Tom Elvis Jedusor - system compatibility updates and Yespower integration (2016)

## Links

Repository: https://github.com/elvisjedusor/bitok

Original Bitcoin: https://bitcoin.org/bitcoin.pdf

Yespower: https://www.openwall.com/yespower/

---

Run the code. That is the manifesto.
