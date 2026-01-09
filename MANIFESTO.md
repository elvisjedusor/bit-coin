# Bitok Manifesto

Bitcoin was never meant to be finished. It was meant to be unleashed.

Satoshi Nakamoto wrote the code, launched the network, fixed critical bugs and then walked away. That was not abandonment. That was completion. A system that depends on its creator is not decentralized.

Bitcoin v0.3.0 was the last release under that principle. Everything after that is history. Not destiny.

## Running Bitok

This is not a proposal. This is not a fork to convince anyone. This is not an argument on a forum.

This is Bitcoin v0.3.0 resurrected.

The same rules. The same behavior. The same philosophy. Adapted only as much as required to run on modern operating systems. No features added. No ideology injected. No attempt to "fix" Bitcoin according to modern tastes.

This is not BTC. It does not compete with BTC. BTC is what Bitcoin became after years of social negotiation and ideological drift.

Bitok is what Bitcoin was before it was captured by its own caretakers.

## Bitcoin Was a Tool, Not a Doctrine

Bitcoin did not begin as an ideology. It began as software that solved a problem: how to transfer value over the internet without asking permission, without identity, without intermediaries, and without trusted parties.

The whitepaper is short because the system was not supposed to be debated into existence. It was supposed to run. And it did.

In v0.3.0, Bitcoin already worked as peer-to-peer electronic cash. Nodes discovered each other. Transactions propagated. Blocks were mined. Wallets generated keys automatically. Addresses were disposable. Balance mattered more than attribution.

Privacy emerged naturally because the software handled complexity for the user instead of lecturing them about "best practices".

There was no concept of layers. No settlement abstraction. No scaling philosophy. No moral judgement about usage. If you created a transaction and it followed the rules, the network accepted it.

Bitcoin did not ask why.

## Satoshi on CPU Mining

On December 12, 2010, Satoshi Nakamoto wrote:

> We should have a gentleman's agreement to postpone the GPU arms race as long as we can for the good of the network. It's much easer to get new users up to speed if they don't have to worry about GPU drivers and compatibility. It's nice how anyone with just a CPU can compete fairly equally right now.

In private correspondence with early developer Laszlo Hanyecz:

> GPUs are much less evenly distributed, so the generated coins only go towards rewarding 20% of the people for joining the network instead of 100%. I don't mean to sound like a socialist, I don't care if wealth is concentrated, but for now, we get more growth by giving that money to 100% of the people than giving it to 20%.

And he concluded:

> It's inevitable that GPU compute clusters will eventually hog all the generated coins, but I don't want to hasten that day.

This was not theoretical concern. It was design philosophy. Mining was meant to be accessible. Anyone with a laptop should be able to participate. The network's security came from distributed participation, not from specialized hardware oligopolies.

Bitcoin abandoned this principle. Bitok restores it.

## The First Corruption Was Governance

After Satoshi stepped back, the code did not immediately become evil. At first, changes were small, practical, and boring. Bug fixes. Platform support. Minor usability improvements.

But something fundamental changed: ownership of direction.

Bitcoin stopped being "software that exists" and became "software that is maintained". Decisions began to be justified not by whether the network functioned, but by whether a group agreed.

From that point on, Bitcoin was no longer just code. It was code plus social process.

That was the seed of capture.

## From Cash to Cathedral

Satoshi described Bitcoin as electronic cash. Not as a settlement layer. Not as a store of value narrative. Not as a base for financial engineering. Cash.

Cash is used. Cash circulates. Cash is not precious because it is untouched.

Modern Bitcoin rejected this. Usage was reframed as a threat. Transactions became "spam". Growth became "abuse". Fees became a filter. Instead of scaling the system to meet demand, demand was moralized away.

Bitcoin was redefined so that most people should not use it directly. This was not forced by physics. It was chosen.

The block size limit was introduced as a temporary safety measure. Satoshi said it could be raised. After he left, it became sacred.

Bitcoin turned inward. The network that was supposed to route around censorship began censoring itself socially.

## Layers Are Control Structures

Layer-2, layer-3, sidechains, hubs. These are not inherently evil. But they are not neutral either.

Bitcoin did not start as a layered system. All transactions were first-class citizens. All users interacted with the same rules.

The moment you push normal usage off-chain, you create new points of coordination, liquidity control, routing policy, and power concentration.

Lightning did not emerge because Bitcoin had to evolve. It emerged because Bitcoin was intentionally constrained.

Instead of making the base system usable, complexity was added around it. Instead of one simple protocol, we now have a stack that only specialists understand.

This is not decentralization. This is abstraction as control.

A system ordinary people cannot reason about is not trustless. It is opaque.

## Wallets Were Never Identities

One of the most damaging lies modern Bitcoin teaches is that a wallet is an address and a private key. That idea did not exist in early Bitcoin.

In v0.3.0, wallets managed pools of keys. Addresses were ephemeral. You did not build identity around them. You did not reuse them. You did not expose your financial graph to the world by default.

Privacy was not a feature. It was an outcome of sane design.

Modern Bitcoin normalized address reuse, static receiving addresses, public donation addresses, and identity-linked keys. Then it blamed users for being tracked.

This inversion is criminal. Bitcoin did not fail at privacy. Bitcoin was redesigned to abandon it.

## Data Is Not Freedom

Script extensions, inscriptions, NFTs, token layers. These are sold as innovation. In reality, they mark a shift away from Bitcoin's original purpose.

Bitcoin's script was intentionally limited. Not because Satoshi lacked imagination, but because complexity is attack surface.

Bitcoin was not meant to be a general data ledger. It was meant to move value reliably.

Turning Bitcoin into a container for arbitrary data does not make it more free. It makes it noisier, more expensive, and easier to surveil. It shifts incentives away from users toward miners extracting rent from block space auctions.

This is not evolution. This is repurposing.

## What Changed From Bitcoin v0.3.0

Bitok is Bitcoin v0.3.0 with exactly three categories of changes:

### 1. Modern System Compatibility

OpenSSL 3.x compatibility (proper BIGNUM handling). Berkeley DB 5.3 C API (C++ bindings no longer exist in modern distros). Boost 1.74+ compatibility. GCC 11+ and C++11 standard compliance. wxWidgets 3.2 support for GUI builds.

These changes allow Satoshi's code to compile and run on Ubuntu 24.04. Zero functional differences. Zero ideological additions.

### 2. GPU-Resistant Proof-of-Work

Removed SHA-256 (GPU/ASIC-friendly). Added Yespower 1.0 (CPU-optimized, memory-hard). N=2048, r=32. Personalization: "BitokPoW". Automatically uses CPU-specific optimizations (SSE2, AVX, AVX2, AVX512).

This change implements what Satoshi asked for but never got: a mining algorithm that keeps GPU miners from hogging all the generated coins.

Anyone with a laptop can mine. No special hardware. No driver hell. No oligopolies.

### 3. New Genesis Block

New genesis block hash: `0x0290400ea28d3fe79d102ca6b7cd11cee5eba9f17f2046c303d92f65d6ed2617`

Separate network from BTC (no interference, no confusion). Same consensus rules, same economics, same peer-to-peer behavior.

That is it. Everything else is exactly as Satoshi wrote it in 2010.

No features added. No protocol extensions. No layers. No abstractions. No "improvements."

## No Leaders, No Roadmap, No Permission

There is no foundation. There is no core team. There is no roadmap.

If you run the software, you are the network. If you don't, you are irrelevant. That is not harsh. That is honest.

This system does not promise mass adoption, regulatory approval, or price appreciation. It promises exactly one thing: that the software will do what it says, and nothing more.

You are responsible for your keys. You are responsible for your node. You are responsible for your mistakes.

That is what freedom looks like in code.

## Masks Matter

Who is Tom Elvis Jedusor? No one.

Systems that require real names are systems that can be shut down. There are no heroes here. Only software and consequences.

## This Is Not Nostalgia

This is not about the past. It is about proving that Bitcoin did not need permission to work then, and does not need it now.

Bitcoin was not broken. It was reinterpreted.

Bitok exists to show that the original code still works. And that freedom did not require layers, councils, or approval.

Run the code. That is the manifesto.

https://github.com/elvisjedusor/bitok

---

## Sources

Satoshi Nakamoto's quotes on GPU mining:
- [Satoshi Nakamoto Institute - Mining Quotes](https://satoshi.nakamotoinstitute.org/quotes/mining/)
- [BitcoinTalk Forum Archive](https://satoshi.nakamotoinstitute.org/posts/bitcointalk/)
