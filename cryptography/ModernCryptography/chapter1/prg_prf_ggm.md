### Why is all this needed? PRG | PRF | GGM

*This article may have inaccuracies; study this to get a general understanding of what it is.*

There is a task: to encrypt data, random bits are needed. Lots of them. Generating true randomness is a slow and expensive thing.
> ...a slow and expensive thing.
>
**Clarification:** This is true for *high-quality* entropy. Modern CPUs have instructions like RDRAND, but for cryptography, multiple entropy sources (timings, ADC noise) are often combined, which indeed requires time to accumulate a sufficient number of bits.

At the same time, in many algorithms (encryption, authentication) we don't need randomness "in a vacuum" — we need the result to *look* random to anyone who doesn't have the secret key.

This gives rise to two basic concepts.

---

### PRG — Stretching Randomness

**Problem**: we have a small amount of "true" randomness (e.g., 256 bits), but need a lot (gigabytes).

**Solution**: take a short random **seed** and pass it through a deterministic algorithm that outputs a long pseudorandom sequence.

**Requirement**: the output must be *computationally* indistinguishable from a truly random string for anyone who doesn't know the seed. No statistical anomalies, no patterns.
> The key word is "computationally" - meaning there is no efficient algorithm that can distinguish them in polynomial time.

**Analogy**: if you are given two sheets with numbers — one with PRG output, the other with true randomness — you won't be able to tell which is which.
> Remember: truly random bits are the gold standard we aim for when creating cryptographic keys and seeds. PRG and PRF are cryptographic primitives that must be computationally indistinguishable from this standard for anyone without the secret key.

**Important**: if the seed is known, the entire pseudorandom sequence can be reproduced. Security relies on the secrecy of the seed and the **strength of the PRG itself**. If the PRG is vulnerable, the output can be predicted even with a secret seed.

---

### PRF — Pseudorandom Functions

**Problem**: we need not just to generate numbers, but to get a "random" result for each input. For example, for message authentication.

**Solution**: a function that, given a key and an arbitrary message, outputs a pseudorandom value.

**Requirement**: even knowing millions of "input-output" pairs, one cannot predict the value for a new input without the key.

**Analogy**: you have a magic box with a setting (the key). For each query, it gives a random-looking answer. Even if you've seen millions of answers to other queries, you won't guess the answer to a new one.

**Important**: PRF is a cryptographic function family that behaves almost indistinguishably from a perfectly random function for anyone without the key. MACs, key derivation schemes, and much more are built on PRFs.

---

### GGM — The Bridge Between PRG and PRF

**Intuition**: if we have a PRG (can stretch randomness), can we make a PRF (a function with arbitrary input) from it?

**It turns out, yes** — via the Goldreich-Goldwasser-Micali tree.

**How it works**:
- Take a PRG that turns one seed into two unpredictable ones
- Build a binary tree where the root is our secret key
- To compute the value for a message `x`, traverse the tree: each bit of `x` tells which branch to take
- The result is the leaf we end up at

**Simpler**: turn the sequence of message bits into a path through the tree, applying the PRG at each step.

**Conclusion**: GGM is primarily a **theoretical construction** that **proves in principle** that any secure PRG can be used to build a secure PRF. In practice, more efficient constructions are used (HMAC, CMAC), but GGM is important for security proofs.
> **Clarification:** In GGM, the tree is not built physically, but virtually — we compute the path on the fly, saving memory.

**PRG** is a static generator. You give it a seed, you get a long predetermined sequence. Everything is predictable in advance after initialization.

**PRF** is a function that can adapt. You give it a key and an arbitrary message — you get a unique result depending on that message.

**GGM** creates this adaptability from a PRG via a tree. Each bit of the input message tells the PRG which branch of the tree to go to next. Thus, different messages lead to different endpoints in the tree.

**PRF** is needed precisely for this adaptability — so that one key can generate different unpredictable results for different inputs, rather than just producing a static data stream like a PRG.

it's needed for *variability*

---

### The Bottom Line

- **PRG** is needed to make a lot of pseudorandom data from a small random seed
- **PRF** is needed to get deterministic but random-looking results for any inputs
- **GGM** shows that these concepts are related: having a PRG, you can build a PRF

All modern cryptography uses these ideas in one way or another. When you hear about HMAC, stream ciphers, authentication schemes — behind them are almost always either PRGs, PRFs, or their hybrids.

This is not abstract theory — it's the foundation upon which everything else is built.