### **Deep Packet Inspection (DPI) — deep packet analysis**

**DPI** is a network traffic analysis technology that checks ** not only headers** (like regular firewall/NAT), but also **packet contents** (payload) at the application layer (L7 in the OSI model). It allows you to identify protocols, applications, and even specific services within encrypted traffic.

---

## **1. How does DPI work?**
1. **Full packet analysis**:
- Analyzes **all levels** (Ethernet → IP → TCP/UDP → HTTP/TLS/DNS, etc.).
- Checks **payload**, not just ports/IP.

2. **Analysis methods**:
- **Signature analysis** (search for patterns, for example, "HTTP/1.1").
- **Statistical analysis** (packet size, timestamps).
   - **Machine learning** (detecting VPN/Tor by indirect signs).
   - **Analysis of TLS metadata** (SNI, ALPN, certificates).

3. **Detection examples**:
- Detection of Netflix in HTTPS traffic.
   - Blocking Telegram even through a proxy.
   - Identification of Tor traffic based on characteristic patterns.

---

## **2. Where is DPI applied?**
| Scenario            | Who is using              | Goal                               |
| ------------------- | ------------------------- | ---------------------------------- |
| **Censorship**      | Roskomnadzor, China (GFW) | Blocking VPN, Tor, Banned sites    |
| **Security**        | Corporate networks        | Detection of attacks, data leaks   |
| **Traffic shaping** | Providers                 | Speed limit for YouTube/BitTorrent |
| **Advertising**     | Google, Facebook          | User behavior Analysis             |

---

## **3. How to bypass DPI?**
### **1. Traffic masking (Obfuscation)**
- **Protocol Hiding**:
- **Tor obfs4** — disguising as regular HTTPS.
  - **ShadowSocks** — Traffic looks like random noise.
  - **Meek** — hides in cloud services (Azure, Cloudflare).

- **Fake metadata**:
- **Fake SNI** (`cloudflare.com `instead of `piratebay.org `).
  - **Non-standard ports** (HTTPS on 443 vs. 8443).

### **2. Eliminating signatures**
- **Padding** (as in your code) — adding garbage to packages.
- **Fragmentation** — splitting data into parts so that there are no patterns.
- **Invisible protocols**:
- **I2P** is a completely hidden stack.
  - **Freenet** is a P2P network without centralized servers.

### **3. Full encryption**
- **TLS 1.3 + ESNI** — hides the site name from the provider.
- **QUIC** (HTTP/3) — harder to analyze because of UDP.
- **Self—signed certificates** - so that there is no JA3 fingerprint analysis.

---

## **4. Example: How DPI detects Tor**
1. **TLS Analysis**:
- Tor uses non-standard TLS extensions.
   - Node certificates have unique fields.

2. **Time patterns**:
- Tor creates chains of 3 jumps → delays of 300-600 ms.

3. **Packet size**:
- The first packets when connected are always 514 bytes.

**How Tor gets around this**:
- **Obfs4** adds random data to the beginning of the session.
- **Meek** uses CDN as a proxy.

---

## **5. DPI vs. Your code**
If you send encrypted data without decryption on intermediate nodes:
- **DPI will not see the contents**, but it can:
- Block traffic by **packet size**.
  - Suspect VPN by **temporary characteristics**.

**What to do**:
1. Add **random padding** (as in your code).
2. Use **non-standard ports** (not 80/443).
3. Disguise as **legitimate protocol** (for example, simulate WebSocket).

---

## **6. DPI Testing Tools**
1. **Wireshark** + **L7-filter** — pattern analysis.
2. **nDPI** (from Cisco) — detects 300+ protocols.
3. **QoS Pirate** — verification of locks by the provider.

---

### **Conclusion**
DPI is a powerful censorship and monitoring tool. For protection:
- **Encrypt everything** (even if it seems redundant).
- **Mask metadata** (SNI, ports, packet size).
- **Use obfsproxy** if traffic is blocked.