**I. Introduction and Classical Cryptography**

1.  Introduction
    *   Cryptography and Modern Cryptography
    *   The Setting of Private-Key Encryption
    *   Historical Ciphers and Their Cryptanalysis
    *   Principles of Modern Cryptography
        *   Principle 1 – Formal Definitions
        *   Principle 2 – Precise Assumptions
        *   Principle 3 – Proofs of Security
        *   Provable Security and Real-World Security
    *   References and Additional Reading
    *   Exercises

2.  Perfectly Secret Encryption
    *   Definitions
    *   The One-Time Pad
    *   Limitations of Perfect Secrecy
    *   \*Shannon’s Theorem
    *   References and Additional Reading
    *   Exercises

**II. Private-Key (Symmetric) Cryptography**

3.  Private-Key Encryption
    *   Computational Security
        *   The Concrete Approach
        *   The Asymptotic Approach
    *   Defining Computationally Secure Encryption
        *   The Basic Definition of Security (EAV-Security)
        *   \*Semantic Security
    *   Constructing an EAV-Secure Encryption Scheme
        *   Pseudorandom Generators
        *   Proofs by Reduction
        *   EAV-Security from a Pseudorandom Generator

4.  Message Authentication Codes
    *   Message Integrity
        *   Secrecy vs. Integrity
        *   Encryption vs. Message Authentication
    *   Message Authentication Codes (MACs) – Definitions
    *   Constructing Secure Message Authentication Codes
        *   A Fixed-Length MAC
        *   Domain Extension for MACs
    *   CBC-MAC
        *   The Basic Construction
        *   \*Proof of Security
    *   GMAC and Poly1305
    *   MACs from Difference-Universal Functions
    *   Instantiations
    *   \*Information-Theoretic MACs
        *   One-Time MACs from Strongly Universal Functions
        *   One-Time MACs from Difference-Universal Functions
    *   Limitations on Information-Theoretic MACs
    *   References and Additional Reading
    *   Exercises

5.  CCA-Security and Authenticated Encryption
    *   Chosen-Ciphertext Attacks and CCA-Security
        *   Padding-Oracle Attacks
        *   Defining CCA-Security
    *   Authenticated Encryption
    *   CCA-Security vs. Authenticated Encryption
    *   Authenticated Encryption Schemes
        *   Generic Constructions

6.  Hash Functions and Applications
    *   Definitions
        *   Collision Resistance
        *   Weaker Notions of Security
    *   Domain Extension: The Merkle-Damgard Transform
    *   Message Authentication Using Hash Functions
        *   Hash-and-MAC
        *   HMAC
    *   Generic Attacks on Hash Functions
        *   Birthday Attacks for Finding Collisions
        *   Small-Space Birthday Attacks
        *   \*Time/Space Tradeoffs for Inverting Hash Functions
    *   The Random-Oracle Model
        *   The Random-Oracle Model in Detail
        *   Is the Random-Oracle Methodology Sound?
    *   Additional Applications of Hash Functions
        *   Fingerprinting and Deduplication
        *   Merkle Trees
        *   Password Hashing
        *   Key Derivation
        *   Commitment Schemes
    *   References and Additional Reading
    *   Exercises

7.  Practical Constructions of Symmetric-Key Primitives
    *   Stream Ciphers
        *   Linear-Feedback Shift Registers
        *   Adding Nonlinearity
        *   Trivium
        *   RC4
        *   ChaCha20
    *   Block Ciphers
        *   Substitution-Permutation Networks
        *   Feistel Networks
        *   DES – The Data Encryption Standard
        *   3DES: Increasing the Key Length of a Block Cipher
        *   AES (Advanced Encryption Standard)
        *   \*Differential and Linear Cryptanalysis
    *   Compression Functions and Hash Functions
        *   Compression Functions from Block Ciphers

8.  \*Theoretical Constructions of Symmetric-Key Primitives
    *   One-Way Functions
        *   Definitions
        *   Candidate One-Way Functions
        *   Hard-Core Predicates
    *   From One-Way Functions to Pseudorandomness
    *   Hard-Core Predicates from One-Way Functions
        *   A Simple Case
        *   A More Involved Case
        *   The Full Proof

9.  Public-Key (Asymmetric) Cryptography
    *   Number Theory and Cryptographic Hardness Assumptions
        *   Preliminaries and Basic Group Theory
            *   Primes and Divisibility
            *   Modular Arithmetic
            *   Groups
            *   The Group ZN\*
            *   \*Isomorphisms and the Chinese Remainder Theorem
        *   Primes, Factoring, and RSA
            *   Generating Random Primes
            *   \*Primality Testing
            *   The Factoring Assumption
            *   The RSA Assumption
            *   \*Relating the Factoring and RSA Assumptions
        *   Cryptographic Assumptions in Cyclic Groups
            *   Cyclic Groups and Generators
            *   The Discrete-Logarithm/Diffie-Hellman Assumptions
            *   Working in (Sub)groups of Zp\*
            *   Elliptic Curves

10. \*Algorithms for Factoring and Computing Discrete Logarithms
    *   Algorithms for Factoring
        *   Pollard’s ρ Algorithm
        *   Pollard’s p-1 Algorithm
        *   The Quadratic Sieve Algorithm
    *   Generic Algorithms for Computing Discrete Logarithms
        *   The Pohlig-Hellman Algorithm
        *   The Baby-Step/Giant-Step Algorithm
        *   Discrete Logarithms from Collisions
    *   Index Calculus: Computing Discrete Logarithms in Zp\*
    *   Recommended Key Lengths
    *   References and Additional Reading
    *   Exercises

11. Key Management and the Public-Key Revolution
    *   Key Distribution and Key Management
    *   A Partial Solution: Key Distribution Centers
    *   Key Exchange and the Diffie-Hellman Protocol
    *   The Public-Key Revolution
    *   References and Additional Reading
    *   Exercises

12. Public-Key Encryption
    *   Public-Key Encryption – An Overview
    *   Definitions
    *   Security against Chosen-Plaintext Attacks
    *   Multiple Encryptions
    *   Security against Chosen-Ciphertext Attacks
    *   Hybrid Encryption and the KEM/DEM Paradigm
    *   CPA-Security
    *   CCA-Security
    *   CDH/DDH-Based Encryption
    *   El Gamal Encryption
    *   DDH-Based Key Encapsulation
    *   \*A CDH-Based KEM in the Random-Oracle Model
    *   \*Chosen-Ciphertext Security and DHIES/ECIES
    *   RSA-Based Encryption
    *   Plain RSA Encryption
    *   Padded RSA and PKCS #1 v1.5
    *   \*CPA-Secure Encryption without Random Oracles
    *   OAEP and PKCS #1 v2
    *   \*A CCA-Secure KEM in the Random-Oracle Model
    *   RSA Implementation Issues and Pitfalls
    *   References and Additional Reading
    *   Exercises

13. Digital Signature Schemes
    *   Digital Signatures – An Overview
    *   Definitions
    *   The Hash-and-Sign Paradigm
    *   RSA-Based Signatures
        *   Plain RSA Signatures
        *   RSA-FDH and PKCS #1 Standards
    *   Signatures from the Discrete-Logarithm Problem
        *   Identification Schemes and Signatures
        *   The Schnorr Identification/Signature Schemes
        *   DSA and ECDSA
    *   Certificates and Public-Key Infrastructures
    *   Putting It All Together – TLS
    *   \*Signature
    *   References and Additional Reading
    *   Exercises

14. \*Post-Quantum Cryptography
    *   Post-Quantum Symmetric-Key Cryptography
        *   Grover’s Algorithm and Symmetric-Key Lengths
        *   Collision-Finding Algorithms and Hash Functions
    *   Shor’s Algorithm and its Impact on Cryptography
    *   Post-Quantum Public-Key Encryption
    *   Post-Quantum Signatures
        *   Lamport Signature Scheme
        *   Chain-Based Signatures
        *   Tree-Based Signatures
    *   References and Additional Reading
    *   Exercises

15. \*Advanced Topics in Public-Key Encryption
    *   Public-Key Encryption from Trapdoor Permutations
        *   Trapdoor Permutations
        *   Public-Key Encryption Schemes
    *   The Paillier Encryption Scheme
        *   The Structure of ZN\*
        *   The Paillier Encryption Scheme
        *   Homomorphic Encryption
    *   Secret Sharing and Threshold Encryption
        *   Secret Sharing
        *   Verifiable Secret Sharing
        *   Threshold Encryption and Electronic Voting
    *   The Goldwasser-Micali Encryption Scheme
        *   Quadratic Residues Modulo a Prime
        *   Quadratic Residues Modulo a Composite
        *   The Quadratic Residuosity Assumption
    *   The Rabin Encryption Scheme
        *   Computing Modular Square Roots
        *   A Trapdoor Permutation Based on Factoring
        *   The Rabin Encryption Scheme
    *   References and Additional Reading
    *   Exercises

**Appendix A Mathematical Background**
* Identities and Inequalities
* Asymptotic Notation
* Basic Probability
* The "Birthday" Problem
* \*Finite Fields

**Appendix B Basic Algorithmic Number Theory**
* Integer Arithmetic
    * Basic Operations
    * The Euclidean and Extended Euclidean Algorithms
* Modular Arithmetic
    * Basic Operations
    * Computing Modular Inverses
    * Modular Exponentiation
    * \*Montgomery Multiplication
    * Choosing a Uniform Group Element
* \*Finding a Generator of a Cyclic Group
    * Group-Theoretic Background
    * Efficient Algorithms

**References**
**Index of Common Notation**
**Index**

**Key:**

*   Sections marked with an asterisk (\*) are considered more advanced topics.
