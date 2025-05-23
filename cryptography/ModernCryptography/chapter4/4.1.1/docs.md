### Summary: The main goals of cryptography and ensuring the integrity of messages  

#### **1. The main purpose of cryptography**  
Cryptography ensures **secure data transfer** between the parties.  
- **Transmission security** includes:  
  - **Secrecy** – protection against passive interception (eavesdropping).  
  - **Integrity (authenticity) messages** – protection against active attacks (substitution, modification).  

#### **2. Security issues unrelated to secrecy**  
Even if the data is encrypted, an attacker can:  
- **To introduce false messages** (impersonation).      
- **Modify the transmitted data** (tampering).  

##### **Example 1: Bank transfer**  
The user sends a request to transfer $1000. The bank must make sure:  
1. **Authenticity of the request** – was it really sent by the account holder?
2. **Immutability of the request** – was the recipient or the amount changed during the transfer?  

→ **Conventional error correction (ECC) methods do not help**, as they are designed for accidental errors, not malicious changes.  

##### **Example 2: Web cookies**  
HTTP is a stateless protocol, so session data (for example, a shopping cart) is stored in **cookies** on the user's side.  
- Problem: the user can **change the cookie content** (for example, reduce the prices of goods).  
- Requires **data integrity**, but not secrecy (the user must see the content).  

#### **3. Why is integrity important?**  
- Any data transmitted without protection (e-mail, SMS, online payments) can be:
- Forged (spoofing).  
  - Changed during the transfer (tampering).  
- People often trust superficial signs (for example, caller-ID, sender's email address), but they are easy to fake.  

#### **4. Solution: Cryptographic methods for ensuring integrity**  
- **It is impossible to completely prevent** tampering or modification at the physical level.  
- But it is possible to **detect** such actions using cryptography.  

**Basic methods:**  
1. **Message Authentication Codes (MAC – Message Authentication Codes).**  
2. **Digital signatures.**  
3. **Hash functions** (in combination with other methods).  

#### **5. Conclusion**  
- **Secrecy, Security.** Even if the data is encrypted, it needs to be protected from forgery.  
- **Integrity** is just as important as confidentiality.  
- Cryptography provides tools to **guarantee the authenticity and immutability of data**.  
  