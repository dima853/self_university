# 2.1 Classes of Links
[Digital Subscriber	Line (DSL)](https://en.wikipedia.org/wiki/Digital_subscriber_line)
### **Digital Subscriber Line (DSL) — Digital subscriber line technology**

**DSL** is a family of technologies that provide high—speed Internet access via conventional telephone lines (copper pairs). Unlike dial-up, DSL allows simultaneous transmission of data and voice (telephone conversations) due to frequency separation.

---

## **1. How DSL works**
### **Key Features**
- **Uses the existing infrastructure** of telephone lines (copper pairs).
- **Operates at frequencies above the voice range** (25 kHz – 2.2 MHz), so it does not interfere with telephone communication.
- **Asymmetric and symmetric transmission**:
- **ADSL** (Asymmetric DSL) — different download and upload speeds.
  - **SDSL** (Symmetrical DSL) — the same speed in both directions.

### **Technical basis**
- **Signal modulation**:
The following methods are used:
- **DMT (Discrete Multi-Tone)** — splitting the signal into 256 frequency channels (for ADSL2+).
  - **CAP (Carrierless Amplitude Phase)** — deprecated method (early versions of DSL).
- **Filtering (Splitter)**:
- **Low-pass filter** — skips the voice (0-4 kHz).
  - **High-pass filter** — highlights data (above 25 kHz).

---

## **2. Types of DSL technologies**
| **Type**       | **Speed (Download/Upload)** | **Application**                     |
|---------------|-------------------------------|-----------------------------------|
| **ADSL**      | Up to 24 Mbit/s / 1.4 Mbit/s | Home Internet (obsolete)    |
| **ADSL2+**    | Up to 24 Mbit/s / 3.3 Mbit/s | Improved ADSL version |
| **VDSL**      | Up to 100 Mbit/s / 50 Mbit/s | Short lines (<1.2 km) |
| **VDSL2** | Up to 200 Mbps / 100 Mbps | Fiber Optic Infrastructure (FTTC) |
| **SDSL**      | Up to 2.3 Mbit/s (symmetric)   | Business connections (deprecated)       |
| **G.fast** | Up to 1 Gbit/s (per 100 m) | VDSL2 replacement (Gigabit speeds)|

---

## **3. Advantages and disadvantages**
### **✅ Advantages**
- Uses **existing telephone lines** (cheaper than deploying fiber).
- **Does not interfere with telephone communication** (voice and internet work simultaneously).
- **Available in rural areas** (where there is no fiber).

### **❌ Disadvantages**
- **The speed depends on the distance**:
- ADSL: >5 km from the PBX → the speed drops to 1-2 Mbps.
  - VDSL: >1.5 km → sharp decrease in speed.
- **Inferior to optical fiber (FTTH)** and cable Internet (DOCSIS 3.1).
- **Asymmetry** (ADSL upload is much slower than download).

---

## **4. Where is DSL used today?**
1. **"Last Mile"** in regions without fiber.
2. **Combined Solutions**:
- **FTTC (Fiber to the Cabinet)** — optical fiber to the control cabinet + VDSL2 to the subscriber.
   - **G.fast** — for apartment buildings (gigabit speeds over short distances).
3. **Backup communication channels** (in business networks).

---

## **5. Comparison with other technologies**
| **Parameter**       | **DSL (VDSL2)** | **Cable (DOCSIS 3.1)** | **Optical Fiber (FTTH)** |
|--------------------|----------------|---------------------------|-----------------------|
| **Max. Speed** | 200 Mbps | 1 Gbps | 10 Gbps+            |
| **Distance dependence** | Strong | Weak | None |
| **Phone Compatibility** | Yes|No (VoIP required) | No (VoIP) |
| **Cost of deployment** | Low | Medium | High |

---

## **6. The future of DSL**
- **G.fast** is the latest evolution of DSL (up to 1 Gbit/s per 100-200 m).  
- **Displacement by optical fiber** (FTTH) in cities.  
- **Niche application** in rural areas and as a temporary solution.  

### **Conclusion**
DSL played a key role in the transition from dial-up to broadband Internet, but today it is inferior to more modern technologies. However, in combination with FTTC (fiber to the cabinet), it remains relevant for millions of users.

# [Coaxial Cable](https://en.wikipedia.org/wiki/Coaxial_cable?ysclid=majkf3rczi300781000)

Coaxial cable (or "coax") is a type of electrical cable widely used for transmitting high-frequency signals with low interference. It consists of multiple layers that help shield the signal from external noise.

---

## **1. Structure of Coaxial Cable**  
A coaxial cable has four main components:  

1. **Center Conductor** – A solid or stranded copper wire that carries the signal.  
2. **Dielectric Insulator** – A non-conductive layer (usually plastic) that separates the conductor from the shield.  
3. **Metallic Shield** – A braided or foil layer that blocks electromagnetic interference (EMI).  
4. **Outer Jacket** – A protective plastic coating that prevents physical damage.  

```
[Outer Jacket]
      |
[Metallic Shield]
      |
[Dielectric Insulator]
      |
[Center Conductor]
```

---

## **2. Types of Coaxial Cables**  
Different types are used for specific applications:  

| **Type**       | **Impedance** | **Common Uses**                     |
|---------------|-------------|-----------------------------------|
| **RG-6**      | 75 Ω        | Cable TV, Satellite, Broadband     |
| **RG-8**      | 50 Ω        | Radio antennas, Amateur radio      |
| **RG-58**     | 50 Ω        | Ethernet (older networks), CB radio |
| **RG-59**     | 75 Ω        | CCTV, Analog video                 |
| **LMR-400**   | 50 Ω        | High-power RF, Cellular antennas  |

---

## **3. Advantages of Coaxial Cable**  
✔ **Low Signal Loss** – Good for long-distance transmissions.  
✔ **High Bandwidth** – Supports high-frequency signals (up to several GHz).  
✔ **EMI Shielding** – Reduces interference from external sources.  
✔ **Durable** – Resistant to physical damage and weather conditions.  

---

## **4. Disadvantages**  
✖ **Thicker & Less Flexible** – Harder to install in tight spaces.  
✖ **More Expensive** Than UTP (Ethernet) cables.  
✖ **Limited Data Speed** – Slower than fiber optics for modern broadband.  

---

## **5. Common Applications**  
- **TV & Internet (Cable & Satellite)**  
- **Radio & Antenna Systems**  
- **CCTV & Security Cameras**  
- **Military & Aerospace Communications**  
- **Medical Imaging Equipment**  

---

## **6. Coaxial vs. Fiber Optic vs. Ethernet**  

| Feature        | Coaxial | Fiber Optic | Ethernet (UTP) |
|--------------|--------|------------|--------------|
| **Speed**    | Up to 10 Gbps | Up to 100+ Gbps | Up to 10 Gbps |
| **Distance** | ~500m | 40+ km | ~100m |
| **Interference** | Low | None | Moderate |
| **Cost** | Medium | High | Low |

---

## **7. Future of Coaxial Cables**  
While fiber optics are replacing coax in high-speed internet, coaxial cables remain relevant for:  
- **Hybrid Fiber-Coaxial (HFC) networks** (used by ISPs like Comcast)  
- **Legacy systems** (TV, radio, CCTV)  
- **RF & Microwave transmissions**  

### **Conclusion**  
Coaxial cables are reliable for RF signal transmission but are gradually being replaced by fiber in high-speed data networks. However, they remain essential in broadcasting, surveillance, and telecom industries.  


### The formula for calculating the wavelength in a copper cable:
$$[
\text{Wavelength} = \frac{\text{The speed of light in copper}}{\text{Frequency}} = \frac{\frac{2}{3} \times 3 \times 10^8}{300}
\]$$

### Step-by-step calculation:
1. **The speed of light in copper** (~66% of the speed in vacuum):  
   $$\[
   \frac{2}{3} \times 3 \times 10^8 = 2 \times 10^8 \, \text{m/s}
   \]$$

2. **Frequency substitution** (300 Hz):
$$[
   \frac{2 \times 10^8}{300} = \frac{2}{3} \times 10^6 \approx 667 \times 10^3 \, \text{meters}
   \]$$

### The final result:
$$\[
\boxed{667\, \text{km} \quad \text{(667,000 meters)}}
\]$$

#### Note:
- For high frequencies (e.g. 300 MHz), the wavelength will be significantly shorter.
- Signals propagate slower in copper media than in vacuum (coefficient ~2/3).
> Generally, electromagnetic waves span	a much wider range of frequencies, ranging from	radio waves, to	infrared
light, to visible light, to	x-rays and gamma rays.	

| Frequency (Hz) | Type of radiation | Applications |
|--------------|---------------------|---------------------|
| 10^0         | Radio               | Coax                |
| 10^2         | Microwave           | AM                  |
| 10^4         | Infrared            | FM                  |
| 10^6         | UV                  | TV                  |
| 10^8         | X-ray               | Terrestrial microwave|
| 10^10        | Gamma ray           | Fiber optics        |
> Such links provide the basis for transmitting all kinds of information, including the data that we are interested in
transmitting — binary data (1 and 0).

**The problem of encoding binary data onto electromagnetic signals is a	complex	topic.**

# **Summary: Fundamentals of modulation and data transmission**  

#### **1. Lower level (modulation)**  
- **Modulation** is the change of signal parameters (frequency, amplitude, phase) to transmit information.  
- **Example**: Amplitude modulation (AM) is a change in signal power (analogous to turning on/off a light).  
- **Application**: Conversion of digital data into analog signals for transmission over physical media (copper wires, radio waves).  

#### **2. Upper level (data encoding)**  
- **Task**: Converting binary data (0 and 1) into distinguishable signals (for example, "high" and "low" levels).  
- **Simplification**: In the context of computer networks, modulation is considered a solved problem; the focus is on coding methods.  

#### **3. Key ideas**  
1. **Separation of layers**:
- **Bottom layer** (physical): signal modulation.  
   - **Top layer** (channel/network): encoding bits into signals.  
2. **Binary encoding**:  
   - Uses two distinguishable signal states to represent 0 and 1.  
   - Examples of methods: NRZ, Manchester coding (see the next section).  
3. **Abstraction**:  
   - For network design, the modulation is often "hidden", and the analysis is carried out at the level of discrete signals.  

#### **4. Importance for networks**  
- **Reliability**: Correct encoding minimizes transmission errors.  
- **Compatibility**: Standardized methods (for example, Ethernet) ensure the interaction of devices.  

---

### **Conclusion**  
Modulation and encoding are fundamental data transmission processes. In network models, the upper layer (encoding) plays a key role, abstracting from the physical implementation of the modulation.  

For in-depth study:
- **Modulation methods**: AM, FM, PSK.  
- **Coding schemes**: NRZ, 4B/5B, Manchester code.

# **Summary: Classification of network connections by type of use**

#### **1. Main categories of network connections**
1. **Wireless Networks**
- **Where**meet: cafes, airports, universities, public places.  
   - **Features**:
- Convenience for mobile users.  
     - Limited speed and coverage area.  
     - Examples: Wi-Fi, 4G/5G.  

2. **"The Last Mile" (Last-Mile Links)**
- **Providers**: Internet services for end users (homes, offices).  
   - **Technologies**:  
     - **DSL** (uses existing copper telephone lines).  
     - **Cable Internet** (via CATV coaxial cables).  
     - **Optical Fiber (FTTH)** — High-speed connection.  
   - **Features**:
- Cost-effective for mass deployment.  
     - They are not suitable for creating backbone networks.  

3. **Backbone Links**
- **Destination**: Connecting between cities and countries.  
   - **Technologies**:
- High-speed fiber optic lines.  
     - Satellite or radio relay communications (for remote regions).  
   - **Requirements**:  
     - High bandwidth (100+ Gbps).  
     - Minimal delays.  

---

#### **2. Economic and technological factors**
- **DSL**: Cheap to deploy (uses existing telephone infrastructure), but outdated.  
- **Optical Fiber (FTTH)**: More expensive, but provides future scalability.  
- **Wireless technologies**: Low cost coverage for mobile users, but depend on the frequency spectrum.  

---

#### **3. The limitations of the "last mile"**
- **Not suitable for global networks**:  
  - Low speed compared to main lines.  
  - Local coverage (for example, DSL only works at a distance of ~5 km from the PBX).  
- **Solution**: A combination of "last mile" + high-speed highways.  

---

### **Conclusion**
Network connections are classified according to **place of use** (user vs backbone) and **economic feasibility**.  
- **For consumers**: Wireless and "last mile" (DSL, Cable, FTTH).  
- **For global networks**: Fiber optic trunks.  

**Example**: Home Wi-Fi (wireless) → DSL (last mile) → Urban Fiber (backbone).  

For in-depth study:  
- **Comparison of "last mile" technologies**: DSL vs Cable vs FTTH.  
- **Current trends**: Deployment of 5G and satellite Internet (Starlink).

| Service               | Bandwidth (typical)  |
|-----------------------|----------------------|
| Dial-up               | 28-56 kbps           |
| ISDN                  | 64-128 kbps          |
| DSL                   | 128 kbps-100 Mbps    |
| CATV (cable TV)       | 1-40 Mbps            |
| FTTH (fiber to home)  | 50 Mbps-1 Gbps       |

_Table 1.Common services available to connect your home._

# **Summary: Modern network connection technologies**

---

#### **1. Long-term backbone connections (Backbone Links)**
- **Main technology**: **Optical fiber** (almost completely replaced coaxial cables in the last 20-30 years).  
- **Standard**: **SONET (Synchronous Optical Network)**
- Designed for telecommunication operators.  
  - Provides:
- High reliability and manageability.  
    - Synchronous data transmission (minimum delays).  
  - Modern analogues: **DWDM** (dense multiplexing) to increase bandwidth.  

---

#### **2. Local Area Networks (LANs)**
- **Dominant technology**: **Ethernet**
- Replaced outdated solutions (for example, Token Ring).  
  - Modern standards:  
    - **Wired**: Gigabit Ethernet (1/10/100 Gbps).  
    - **Wireless**: **Wi-Fi (802.11 a/b/g/n/ac/ax)**.  
- **Trends**:
- Combination of Ethernet and Wi-Fi in corporate and home networks.  
  - Increased speed (up to 100 Gbit/s for wired, up to 9.6 Gbit/s for Wi-Fi 6).  

---

#### **3. Diversity of technologies and their integration**
- **Reasons for diversity**:  
  - Economic feasibility (for example, DSL for the "last mile").  
  - Technical requirements (optical fiber for highways, Wi-Fi for mobility).  
- **The task of network protocols**:
- Hide the low-level complexity (different types of connections).  
  - Provide a single interface for the upper layers (for example, the IP layer).  

---

### **Key findings**
1. **Optical fiber** is the standard for highways due to speed and reliability (SONET/DWDM).  
2. **Ethernet and Wi-Fi** dominate LAN, providing flexibility and high performance.  
3. **Network protocols** (for example, TCP/IP) abstract the differences between technologies, simplifying the work of applications.  

---

### **Additional topics to study**
- **SONET/SDH**: Principles of synchronization and management.  
- **Evolution of Ethernet**: From 10 Mbit/s to Terabit Ethernet.  
- **Wi-Fi 6/7**: New wireless standards.  
- **Software-defined networks (SDN)**: Management of heterogeneous environments.  

This review demonstrates how historical, economic, and technical factors shape modern networks.