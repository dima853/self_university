### **Explanation of the Wi-Fi**  

## **1. What is Wi-Fi?**  
Wi-Fi (Wireless Fidelity) is a wireless data transmission technology based on the standards of the **IEEE 802.11** family. It allows devices to exchange information over a radio channel wirelessly.  

### **1.1. Main features of Wi-Fi**  
- Operates at **2.4 GHz** and **5 GHz** (new standards include **6 GHz**).  
- Uses **radio signal modulation** to encode data.  
- Supports different transfer rates (from 1 Mbit/s in 802.11b to 10+ Gbit/s in 802.11ax).  

---

## **2. Physical Layer (PHY) of Wi-Fi**  
At the lowest level, Wi-Fi is radio waves modulated to transmit bits.  

### **2.1. Frequency ranges and channels**  
- **2.4GHz**: 14 channels (but really 3 non-overlapping: 1, 6, 11).
- **5 GHz**: more channels (20, 40, 80, 160 MHz), less interference.  
- **6 GHz**: New band in Wi-Fi 6E.  

### **2.2. Modulation and coding**  
Data is transmitted using:
- **OFDM** (Orthogonal Frequency-Division Multiplexing) — splitting the signal into subcarriers.  
- **QAM** (Quadrature Amplitude Modulation) — the higher the order (64-QAM, 256-QAM, 1024-QAM), the more bits there are in the symbol.  

Example:  
- 802.11ac (5 GHz) uses **256-QAM**, which gives up to 8 bits/character.
- 802.11ax (Wi-Fi 6) — **1024- QAM** (10 bits/character).  

### **2.3. Transfer rate and MIMO**  
- **MIMO** (Multiple Input Multiple Output) — Using multiple antennas to increase speed.  
- **MU-MIMO** (Multi-User MIMO) — simultaneous transmission to multiple devices.  

---

## **3. Channel Level (MAC) Wi-Fi**  
Access to the transmission medium is controlled at this level.  

### **3.1. CSMA/CA — How Wi-Fi avoids collisions**  
Wired Ethernet uses **CSMA/CD** (collision detection), but you can't "hear" a collision in Wi-Fi, so it applies:  
- **CSMA/CA** (Carrier Sense Multiple Access with Collision Avoidance).  
- Before transmission, the device ** listens to the broadcast** (CCA — Clear Channel Assessment).  
- If the channel is free, it waits for a random time (**DIFS + Backoff**).  

### **3.2. Wi-Fi Frames**  
The data is transmitted as **frames** (frames). The main types are:
1. **Control frames** — RTS, CTS, ACK.  
2. **Service (Management frames)** — Beacon, Probe, Authentication.  
3. **Data (Data frames)** is the actual payload.  

#### **Example of a Beacon frame (from a router):**  
```
| MAC Header | Timestamp | Beacon Interval | Capability Info | SSID | Supported Rates | ... |
```  
The router periodically sends **Beacon frames** so that clients are aware of its existence.  

### **3.3. The process of connecting to Wi-Fi**  
1. **Scanning**
- **Passive Scan**: The client listens to the Beacon frames.  
   - **Active Scan**: The client sends the **Probe Request** himself.  
2. **Authentication**
- Open network: just "hello".  
   - WPA2/WPA3: Complex key exchange (4-way handshake).  
3. **Association**
- The client sends **Association Request**, the router responds with **Association Response**.  

---

## **4. Wi-Fi Security**  
### **4.1. Encryption methods**  
- **WEP (Wired Equivalent Privacy)** — hacked, not in use.  
- **WPA (Wi-Fi Protected Access)** — TKIP is already unreliable.  
- **WPA2** — AES-CCMP, the standard for a long time.  
- **WPA3** — more resistant (SAE instead of PSK, protection against brute force attacks).  

### **4.2. 4-way handshake in WPA2**  
1. **ANonce** (from the router) →  
2. **SNonce + MIC** (from the client) →  
3. **GTK + MIC** (from the router) →  
4. **ACK** (from the client).  
If an attacker intercepts the handshake, they may try to find the password.  

---

## **5. Wi-Fi 6 (802.11ax) and new technologies**  
- **OFDMA** — channel splitting into subchannels for multiple clients.  
- **BSS Coloring** — marking networks to reduce interference.  
- **TWT (Target Wake Time)** — reduction of power consumption of IoT devices.  

---

## **6. English-language articles on Wi-Fi architecture**  
1. **[Wi-Fi Architecture Deep Dive (IEEE)](https://www.ieee.org/)**  
2. **[802.11ax Explained (Wi-Fi 6)](https://www.wi-fi.org/)**  
3. **[How Wi-Fi Works at Low Level (Arxiv)](https://arxiv.org/)**  
4. **[Wi-Fi Security: WPA3 and Beyond](https://www.usenix.org/)**  

---

### **Conclusion**  
Wi-Fi is a complex technology that operates on several levels, from radio waves to cryptography. Modern standards (Wi-Fi 6/6E/7) make it faster and more reliable. If you want to go even deeper, study the IEEE 802.11 documentation and Linux driver sources (for example, `mac80211' in the kernel).