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
  - Public point `Q = k * G' (where `k` is the secret key).  
- **Goal:** Find `k'.  

### **Pollard's Rho for ECC**
The algorithm is adapted to **search for collisions** in sequences of points:  
1. **We generate a "turtle" and a "hare"** in a group of points.  
2. **We are looking for a collision** of the form:
``
   a₁ * G + b₁ * Q = a₂ * G + b₂ * Q
   ```  
   Then:
``
(A₁ - a₂) * G = (b₂ - b₁) * Q = (b₂ - b₁) * k * G
   ```  
   From here:
``
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