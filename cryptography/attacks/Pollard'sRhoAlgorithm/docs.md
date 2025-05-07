# **1. The attack on RSA (factorization of the modulus `n`)**
**RSA** is based on the fact that it is difficult to decompose a large number `n = p * q` (where `p` and `q` are prime numbers).  
**Pollard's Rho** can speed up hacking if the `p` or `q` are **not large enough**.

### **How does it work?**
1. **Given:**
- RSA public key: `(n, e)`.  
   - `n` is the product of two large prime numbers `p` and `q'.  

2. **The target of the attack:**  
   Find `p` and `q` to calculate **the secret key `d`** (using the formula `d ≡ e⁻1 mod (p-1)(q-1)`).  

3. **Apply Pollard's Rho:**
- Run the algorithm for factoring `n`.  
   - If `p` or `q` are **small** (or have **small divisors**), Pollard's Rho will find them **faster than a complete search**.  

### **An example of an attack**
- Let `n = 143` (in practice, `n` should be 2048+ bits, but let's take a small one for example).  
- Run `pollards_rho(143)`:
- Finds the divisor **11** → `143 = 11 * 13`.  
- Now we can calculate `φ(n) = (11-1)*(13-1) = 120` and find `d ≡ e⁻1 mod 120`.  

**Vulnerability:**  
If you select **primes close to each other** or **one of them is small** in RSA, Pollard's Rho will crack the key in minutes/hours instead of millions of years.  

---

# **2. ECC attack (solving the discrete logarithm problem)**
**ECC (Elliptic Curve Cryptography)** is based on the complexity of the discrete logarithm problem in a group of elliptic curve points**.  

### **The discrete logarithm problem in ECC**
- **Given:**
Is the point `G` (generator).  
  - Public point `Q = k * G` (where `k` is the secret key).  
- **Goal:** Find `k'.  

### **Pollard's Rho for ECC**
The algorithm is adapted to **search for collisions** in sequences of points:  
1. **We generate a "turtle" and a "hare"** in a group of points.  
2. **We are looking for a collision** of the form:
```
   a₁ * G + b₁ * Q = a₂ * G + b₂ * Q
   ```  
   Then:
```
(A₁ - a₂) * G = (b₂ - b₁) * Q = (b₂ - b₁) * k * G
   ```  
   From here:
```
   k ≡ (a₁ - a₂) / (b₂ - b₁) mod group_order
   ```  

### **An example of an attack**
- Let `G` be a generator, `Q = 5 * G` (secret key `k = 5`).  
- Pollard's Rho will find a collision and solve the equation for `k'.  

**Vulnerability:**  
If **the order of the group** (the number of points) has **small divisors**, the algorithm speeds up.  

---

# **3. Attack on DSA / ElGamal**
**DSA and ElGamal** are also based on **discrete logarithm**, but in the **multiplicative group ℤₚ***.  

### **How is Pollard's Rho applied?**
1. **Given:**
- `g` — generator.  
   - `y = gˣ mod p` (where `x` is the secret key).  
2. **Goal:** Find `x'.  
3. **Algorithm:**
- Similarly to ECC, we are looking for a collision in sequences of degrees of `g'.  
   - We find `x` through the difference of the indicators.  

### **Example**
- Let's say `p = 19`, `g = 2`, `y = 13`.  
- We need to find an `x` such that `2ˣ ≡ 13 mod 19`.  
- Pollard's Rho will find `x = 5` (since `2⁵ = 32 ≡ 13 mod 19`).  

**Vulnerability:**  
If `p-1` has **small divisors**, the algorithm runs faster.  

---

# **4. How to protect yourself from Pollard's Rho attacks?**
### **For RSA:**
✅ Use **sufficiently large prime numbers** (2048+ bits).  
✅ `p` and `q` must be **far from each other** (so that `n` cannot be factorized via Fermat or Rho).  

### **For ECC / DSA / ElGamal:**
✅ Choose **curves with a large prime order** (for example, NIST P-256).  
 Avoid **curves in which the order of the group is divided into small simple **.  

---

# **Conclusion**
- **Pollard's Rho** is a powerful tool for **factorization** and **discrete logarithm**.  
- It is used for attacks on **RSA, ECC, DSA, ElGamal** if the parameters are selected **insufficiently secure**.  
- **Protection:** Use **large prime numbers** and **curves with good properties**.


# Let's look at ** a real example of hacking RSA** using Pollard's Rho.  

---

## **🔹 Step 1: Generate a "weak" RSA key (for demonstration)**
Let's say Alice chose two **primes** for RSA:
- `p = 104729` (this is a prime)  
- `q = 104723` (also simple)  

She calculates:  
- `n = p * q = 104729 * 104723 = 10 966 061 467`  
- `φ(n) = (p-1)(q-1) = 104728 * 104722 = 10 965 852 016`  
- Selects `e = 65537' (standard open exponent)  
- Calculates `d = e⁻1 mod φ(n) = 65537-1 mod 10 965 852 016`  

**Public key:** `(n=10 966 061 467, e=65537)`  
**Private key:** `d = ...` (we don't know yet)  

---

## **🔹 Step 2: Bob's Attack (hacking through Pollard's Rho)**
Bob intercepts the public key `(n, e)` and wants to find `p` and `q` to calculate `d'.  

### **🔸 1. Run Pollard's Rho to factorize `n`**
In the code from the previous example, we do:
```c
long long n = 10966061467LL;
long long factor = pollards_rho(n);
```
**Result:**  
- The algorithm finds the divisor of `p = 104723` (or `104729`).  
- Now `q = n / p = 10966061467 / 104723 = 104729'.  

### **🔸 2. We calculate `φ(n)` and the secret key `d`**
- `φ(n) = (p-1)(q-1) = 104722 * 104728 = 10 965 852 016`  
- `d = e⁻¹ mod φ(n) = 65537⁻¹ mod 10 965 852 016`  

We calculate the inverse element using the **extended Euclidean algorithm**:  
```c
long long extended_gcd(long long a, long long b, long long *x, long long *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }

    long long x1, y1;
    long long g = extended_gcd(b, a % b, &x1, &y1);

    *x = y1;
    *y = x1 - (a / b) * y1;

    return g;
}

// A function for calculating the modular inverse
long long coding(long long a, long long m) {
    long long x, y;
    long long g = extended_gcd(a, m, &x, &y);

    if (g != 1) {
        return -1; // There is no reverse
    } else {
        return (x % m + m) % m; // Make sure that the result is positive
    }
}

int main() {
    long long d = modinv(65537, 10965852016);

    if (d == -1) {
        printf("The modular inverse does not exist\n");
    } else {
        printf("Modular inverse: %lld\n", d);
    }

    return 0;
}
```
**Result:**  
`d = 3 365 031 713`  

---

## **🔹 Step 3: Bob decrypts the message**
Let's say Alice encrypted the message `m = 123456789` and sent it to Bob:  
`c = mᵉ mod n = 123456789⁶⁵⁵³⁷ mod 10 966 061 467`  

**Bob calculates:**  
`m = cᵈ mod n = c³ ³⁶⁵ ⁰³¹ ⁷¹³ mod 10 966 061 467`  

**And receives the original message `m = 123456789'!**  

---

## **🔹 Why did it work?**
- Pollard's Rho quickly found the divisors of `n` because `p` and `q` **are not big enough** (only 6-digit).  
- In real life, `p` and `q` should be **1024-4096 bits** (300+ digits), and then the algorithm will work **for millions of years**.  

---

## **🔹 How to protect yourself?**
1. **Use large prime numbers** (2048+ bits).  
2. **Check that `p` and `q` are far from each other** (so that it cannot be factorized via Fermat).  
3. **Do not use "bad" cryptographic libraries** that generate weak keys.  

---

## **Conclusion**
- **Pollard's Rho** is a dangerous tool if the RSA keys are **weak**.  
- In reality, for `n=2048 bits` it is **useless**, but for `n=256 bits` it can crack the key in minutes.  
- **Always use recommended standards (NIST, RFC) when generating keys!**

---
---

### **🔐 What is RSA?**
RSA is an **asymmetric cryptographic algorithm** that is used to:  
- **Encryption** (for example, for secure data transmission).  
- **Digital signatures** (verification of the authorship of the message).  

It is based on the complexity **of factoring large numbers** (factorization).  

---

## **🔹 How does RSA work (no code, just math)?**
### **1. Key generation**
Alice wants to receive encrypted messages. She:  

1. **Selects two large prime numbers** `p` and `q' (for example, 1024 bits each).  
   - `p = 104723`  
   - `q = 104729`  

2. **Calculates the module `n`:**  
   \[
   n = p \times q = 104723 \times 104729 = 10\,966\,061\,467
   \]  

3. **Calculates the Euler function `φ(n)`:**  
   \[
   \phi(n) = (p-1)(q-1) = 104722 \times 104728 = 10\,965\,852\,016
   \]  

4. **Selects the open exponent `e`:**
- They usually take `e = 65537` (because it's simple and not too big).  
   - Must be mutually simple with `φ(n)`, that is:
\[
     \gcd(e, \phi(n)) = 1
     \]  

5. **Calculates the secret exponent `d`:**
- `d` is **the inverse of `e` modulo `φ(n)`**, that is:
\[
     d\times e\equiv 1\mod\phi(n)
\]
- Is found via the **extended Euclidean algorithm**.  

**The result:**  
- **Public key (for encryption):** `(n, e)`  
- **Private key (for decryption):** `(n, d)`  

---

### **2. Message encryption**
Bob wants to send Alice the number `m' (for example, `m = 123456789').  

1. **Takes Alice's public key `(n, e)`.**  
2. **Calculates the ciphertext `c`:**  
   \[
   c = m^e \mod n
   \]  
   For example:
\[
c = 123456789^{65537} \mod 10\,966\,061\,467
\]  

3. **Sends 'c` to Alice.**  

---

### **3. Decryption of the message**
Alice receives a `c` and wants to read the original message.  

1. **Takes his private key `(n, d)`.**  
2. **Calculates the original message `m`:**  
   \[
   m = c^d \mod n
   \]  
   For example:
\[
   m = c^{3\,365\,031\,713} \mod 10\,966\,061\,467
   \]  

** Why does it work?**  
Because of **Euler's theorem**:
\[
m^{\phi(n)}\equiv 1\mod n
\]  
Since `d\times e\equiv 1\mod\phi(n)`, then:
\[
c^d\equiv(m^e)^d \equiv m^{e \times d} \equiv m^{1 + k \times \phi(n)} \equiv m \times (m^{\phi(n)})^k \equiv m \times 1^k \equiv m \mod n
\]  

---

### **4. Digital signature**
RSA is also used for **message signing**.  

1. **Alice wants to sign the message with `m`.**  
2. **It calculates the signature `s`:**  
   \[
   s = m^d \mod n
   \]  
3. **Sends `(m, s)` to Bob.**  

4. **Bob verifies the signature:**  
   \[
   m' = s^e \mod n
   \]
- If `m' == m`, then the signature is correct.  

---

## **🔹 Why is RSA secure?**
- **The basis of security:** It is very difficult to decompose `n` into `p` and `q` (if they are large enough).  
- **Best Attacks:**
- **Pollard's Rho** (if `p` or `q` are small).  
  - **Quadratic sieve method** or **NFS** (for large `n`).  
- **Recommended size `n`:** **2048 bits or more** (otherwise it can be hacked).  

---

## **🔹 Where is RSA used?**
1. **SSL/TLS** (Internet connection protection).  
2. **PGP/GPG** (mail encryption).  
3. **Digital signatures** (documents, software).
4. **VPN, SSH, Bitcoin** (in some cases).  

---

## **🔹 Conclusion**
- **RSA** is an **asymmetric algorithm** based on factorization.  
- **Public key `(n, e)`** — for encryption.  
- **Private key `(n, d)`** — for decryption.  
- **Security depends on the size of `n`** (minimum 2048 bits).  
- **Can be hacked** if `p` and `q` are poorly chosen (e.g. Pollard's Rho).