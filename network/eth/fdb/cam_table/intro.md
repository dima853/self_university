<img src="https://upload.wikimedia.org/wikipedia/commons/5/5f/Binary_CAM_cell_schematic.jpg" />


### 🧩 SCHEME DECRYPTION ELEMENT-BY-ELEMENT:

#### 1. LEFT-TO-RIGHT LINES:

```
SLn^    BLn    BLn^    SLn
```

*   `SLn^` (Search Line n NOT) - Search line (inverted)
*   `BLn` (Bit Line n) - Data write line
*   `BLn^` (Bit Line n NOT) - Write line (inverted)
*   `SLn` (Search Line n) - Search line (direct)

**What is the inversion for? ⬇️**

Inversion in CAM is precisely for speed, but at the hardware level. (it can be compared to prefix sums in a state of superposition, as everything happens very quickly)

This is approximately the same principle as prefix sums:

*   Data precomputation (direct + inverted copy)
*   Parallel processing without conditional branches
*   Instant response in O(1) regardless of size!
*   
<img src="https://upload.wikimedia.org/wikipedia/commons/4/43/Threshold_formation_nowatermark.gif" />

> Simulation of formation of inversion channel (electron density) and attainment of threshold voltage (IV) in a nanowire MOSFET. Note: Threshold voltage for this device lies around 0.45 V.

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/4/49/MOS_Capacitor.svg/330px-MOS_Capacitor.svg.png" />

> Metal–oxide–semiconductor structure on p-type silicon

---

#### 🏗️ LAYER STRUCTURE (top to bottom):

1.  **METAL (Металл) - Gate (Затвор):**
    *   `Gate: +VG` (positive voltage)
    *   Material: Aluminum or polysilicon
    *   Purpose: Control electrode — creates an electric field

2.  **OXIDE (Оксид) - Insulator:**
    *   `- Oxide (thickness 1-10 nm)`
    *   Material: SiO₂ (silicon dioxide) — ideal insulator
    *   Purpose: Insulates the gate from silicon but allows the field to pass through

3.  **INVERSION LAYER (Инверсионный слой):**
    *   `- Inversion layer n (electrons)`
    *   What it is: A thin layer of electrons under the oxide
    *   How it forms: Positive voltage on the gate attracts electrons
    *   Key role: This is the conductive channel between the source and drain!

4.  **DEPLETION LAYER (Обедненный слой):**
    *   `- Depletion layer \( N_A \) (depletion zone)`
    *   What it is: A zone where holes are pushed out by the electric field
    *   Forms: A "carpet" of immobile negative ions
    *   Thickness: Depends on the gate voltage

5.  **BULK (Подложка):**
    *   `- Bulk holes p & \( N_A \) (base material)`
    *   Material: P-type silicon
    *   Majority carriers: Holes (p)
    *   Doping: N_A (acceptor atom concentration)

---

#### ⚡ PHYSICS OF OPERATION STEP-BY-STEP:

**STEP 0: INITIAL STATE (VG = 0V)**
*   P-type silicon: many holes (+), few electrons (-)
*   NO inversion layer — only random electrons

**STEP 1: THRESHOLD VOLTAGE (VG > Vth)**
*   Gate: +VG → attracts electrons → repels holes
*   Forms:
    1.  Depletion layer (holes are gone)
    2.  Inversion layer (electrons gathered near the oxide)

**STEP 2: CONDUCTIVE CHANNEL**
*   The inversion layer becomes a BRIDGE between the source and drain!
*   Electrons can flow from left to right — transistor is OPEN!

---

#### 🔁 KEY CONCEPTS:

**CONDUCTIVITY TYPE INVERSION:**
*   Was: P-type (majority carriers — holes)
*   Became: N-type (majority carriers — electrons) at the surface
*   Result: An n-channel was formed in a p-substrate!

**THRESHOLD VOLTAGE (Vth):**
*   `Vth` = the voltage at which the electron concentration at the surface becomes equal to the hole concentration in the bulk.
*   Typically: 0.3-0.7V for modern MOSFETs
*   Depends on: oxide thickness, doping, gate material

---

#### 🔌 HOW THIS WORKS IN CAM TRANSISTORS:

In CAM comparison transistors, the **gate is connected to Q or Q^ from the SRAM!**

*   If `Q = 3.3V`: Gate has high voltage → inversion layer is FORMED
*   If `Q = 0V`: Gate has low voltage → NO inversion layer

**"Wired AND" logic:**
`MLm` will be discharged if:
1.  The transistor gate = 1 (channel is OPEN)
2.  AND the transistor source = 0 (path to ground)

---

#### 2. VERTICAL LINES:

**CENTRAL: `MLm` (Match Line)**
*   Match Line - common to all cells in a row
*   Precharged before a search to Vdd (logical '1')
*   Remains high if ALL cells matched
*   Gets pulled "to ground" if AT LEAST one cell did not match

**RIGHT: `WLm` (Word Line)**
*   Word selection line - activates the entire row for writing
*   When `WLm = '1'` - data can be written via `BLn/BLn^`
*   Usually disabled in search mode

---

#### 🔌 TRANSISTOR SCHEME (BY CHAINS):

**CHAIN 1: LEFT COMPARISON TRANSISTOR**
```
SLn^ ---●---| T1 |--- MLm
        |
       BLn^ (gate connected to Q^ from SRAM)
```
*   Transistor T1: compares `SLn^` with stored `Q^`
*   Opens when: `SLn^ = 1` AND `Q^ = 1`
*   If open → creates a path: `MLm` → ground

**SRAM** — это статическая память, которая хранит бит пока есть питание. Не требует периодического обновления как DRAM.

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/7/77/6t-SRAM-cell.png/800px-6t-SRAM-cell.png" />

*   **DATA STORAGE:**
    *   SRAM cell — это два инвертора в кольцевой схеме:
    *   State 1: `Q = 3.3V`, `Q^ = 0V`
    *   State 0: `Q = 0V`, `Q^ = 3.3V`
*   **Stability:** Инверторы постоянно "поддерживают" друг друга. Если попытаться изменить Q — Q^ усилит противоположное состояние!
*   **Key point:** В режиме поиска SRAM НЕ ЧИТАЕТСЯ традиционным способом! Вместо этого: Напряжения Q и Q^ напрямую управляют затворами транзисторов сравнения!

💡 **WHY SRAM IS IDEAL FOR CAM:**
*   **Staticity:** No refresh needed — data is stable as long as there is power. Ideal for permanent routing tables.
*   **Speed:** Access in 1-2 ns — 10+ times faster than DRAM. Synchronous operation with comparison logic.
*   **Voltage Stability:** Clear 0V/3.3V levels — no intermediate states. Reliable control of comparison transistors.
*   **Low Power in storage mode:** Consumes current only when switching. In static state — almost zero consumption.

`Q` - это НЕ просто бит! Это ФИЗИЧЕСКОЕ НАПРЯЖЕНИЕ:
*   `Q = 1` → voltage ~3.3V on this node
*   `Q^ = 0` → voltage ~0V on the inverse node
*   `Q` and `Q^` — это ПРОТИВОПОЛОЖНЫЕ состояния благодаря инверторам!

**POЧЕМУ ИМЕННО MOSFET:**
MOSFET (Metal-Oxide-Semiconductor FET) идеальны для CAM потому что:

*   **Gate is isolated:**
    ```
    Gate (Gate) ---| |--- Channel
                   Insulator (oxide)
    ```
    No gate leakage current → Q is preserved for years! Field control: voltage is sufficient, current is not needed.
*   **Ideal switches:**
    *   `Gate = 0V` → transistor is closed (resistance ∞)
    *   `Gate = 3.3V` → transistor is open (resistance ~100Ω)

**CHAIN 2: RIGHT COMPARISON TRANSISTOR**
```
SLn ---●---| T2 |--- MLm
       |
      BLn (gate connected to Q from SRAM)
```
*   Transistor T2: compares `SLn` with stored `Q`
*   Opens when: `SLn = 1` AND `Q = 1`
*   If open → creates a path: `MLm` → ground

**CHAIN 3: LEFT WRITE TRANSISTOR**
```
BLn^ ---●---| T3 |--- SRAM_Node_Q^
        |
       WLm (write control)
```
*   Transistor T3: writes data to SRAM when `WLm` is activated

**CHAIN 4: RIGHT WRITE TRANSISTOR**
```
BLn ---●---| T4 |--- SRAM_Node_Q
       |
      WLm (write control)
```
*   Transistor T4: writes inverted data to SRAM

---

#### 🔁 OPERATING PRINCIPLE STEP-BY-STEP:

**WRITE PHASE:**
1.  Activate `WLm = 1` (select word for writing)
2.  Apply data to `BLn/BLn^` (e.g., `BLn=1`, `BLn^=0`)
3.  Transistors T3/T4 open → data is written to the SRAM cell
4.  Deactivate `WLm = 0` → data is saved

**SEARCH PHASE:**
1.  Precharge `MLm = 1` (logic "match exists")
2.  Apply search data to `SLn/SLn^` (e.g., searching for '1' → `SLn=1`, `SLn^=0`)
3.  Parallel comparison in all cells of the row:

**CASE OF MATCH:**
*   Searching for '1', storing '1' → `SLn=1`, `SLn^=0`; `Q=1`, `Q^=0`
*   T1: `SLn^=0`, `Q^=0` → CLOSED
*   T2: `SLn=1`, `Q=1` → SHOULD IT OPEN? 🤔

**STOP! Here's where the dog is buried! Let's figure it out more precisely...**

---

#### 🔍 CORRECTED LOGIC (Key moment):

Actually, the transistor gates are connected to the SRAM, and the sources/drains are between `MLm` and ground through the SL lines!

**Correct connection scheme:**
```
MLm (precharged to Vdd)
 |
●--| T1 |----●---- Ground
 |           |
SLn^        SLn^ (control from Q^)
 
●--| T2 |----●---- Ground  
 |           |
SLn         SLn (control from Q)
```

**CORRECT OPERATING LOGIC:**

**MATCH (searching for '1', storing '1'):**
*   `SLn=1`, `SLn^=0`; `Q=1`, `Q^=0`
*   T1 controlled by `Q^=0` → CLOSED (does not conduct)
*   T2 controlled by `Q=1` → OPEN, but!
*   Circuit: `MLm` → T2 → Ground is NOT CLOSED, because `SLn=1` (high level), not ground!
*   Result: `MLm` remains high ✅

**MISMATCH (searching for '0', storing '1'):**
*   `SLn=0`, `SLn^=1`; `Q=1`, `Q^=0`
*   T1 controlled by `Q^=0` → CLOSED
*   T2 controlled by `Q=1` → OPEN
*   Circuit: `MLm` → T2 → `SLn=0` (GROUND!) → path is closed!
*   Result: `MLm` discharges to ground ❌

💡 **Key insight:**
The SL lines serve not only to transmit search data but also as a "ground source" for discharging `MLm`! This is genius - one line performs two functions!

---

### And what about cam tables in switches?

This is a complete datasheet for a specific switch model — Arista 7050CX3M-32S.

https://www.arista.com/assets/data/pdf/Datasheets/7050X3-macsec-Datasheet.pdf

`Table Sizes` — this is the description of the hardware tables (CAM/TCAM)!

```
UFT Mode - 2 is default   0     1      2      3      4
MAC Addresses           288K  224K   288K   160K   96K
IPv4 Host Routes        16K   80K    144K   168K   16K
```

**What is UFT Mode?**

**UFT (Unified Forwarding Table)** — это технология Arista, которая позволяет гибко распределять ограниченный ресурс аппаратной памяти (TCAM) между разными типами таблиц.

*   **Resource is fixed:** Inside this switch, on the Broadcom chip, there is a certain amount of TCAM memory.
*   **Requests are different:** Some need many MAC addresses (L2 network), while others need many IPv4 routes (L3 network).
*   **UFT solves this:** It allows you to "move the slider" and allocate more memory for the required type of entries.

**How to read this table?**

*   **Rows (`MAC Addresses`, `IPv4 Host Routes`):** These are the types of entries in the hardware table.
*   **Columns (`UFT Mode 0, 1, 2...`):** These are preset memory distribution profiles.
*   **Numbers (`288K`, `144K`):** This is the table capacity — how many entries can be stored in the hardware memory.

**Example:**

In the default mode (`UFT Mode 2`), the switch can store:
*   288,000 MAC addresses (huge L2 table)
*   144,000 IPv4 entries (e.g., ARP table or host routes)

If you need more IP routes, you can switch to `UFT Mode 3` and get 168,000 IPv4 entries, but at the expense of the MAC table size (it will decrease to 160,000).

**Where is the direct link to CAM/TCAM?**

It's not there, and that's normal! "Table Sizes" — это user-friendly название для сетевого инженера. Инженеру Arista, который писал этот даташит, незачем упоминать внутренние термины Broadcom (CAM/TCAM). Он говорит на языке потребителя: "Here is how many entries of different types our switch supports."

These numbers (`288K MAC Addresses`) — это и есть прямое отражение размера и работы аппаратной CAM/TCAM-памяти внутри чипа Broadcom, но выраженное в терминах, понятных тому, кто будет использовать коммутатор.

---

### Questions and Answers (Q&A)

**Q1:** In switches, CAM is used not only for MAC addresses but also for routing tables (IP prefixes), ACLs, and other rules. Why does the article talk specifically about MAC addresses?

**A1:** You are absolutely right. CAM (and its more flexible version, TCAM) is a universal mechanism for high-speed search of any data. We used MAC addresses as the most illustrative example for two reasons:
1.  **Simplicity:** Searching for an exact MAC address match is intuitive. The "match/no match" logic perfectly illustrates the principle of classical CAM operation.
2.  **Specificity:** The size of the MAC address table (e.g., 288K) is a clear and often cited characteristic of a switch that allows for a performance assessment.

**Q2:** The number 288,000 looks very specific. Is the table size always exactly that?

**A2:** No, this value depends on the equipment model and, more importantly, on the chip's operating mode. For example, modern switches use UFT (Unified Forwarding Table) technology, which allows flexible redistribution of the limited hardware memory resource between different types of entries (MAC addresses, IPv4 routes, IPv6 entries). Thus, in one mode, you can get 288,000 MAC address entries, and in another, you can sacrifice some of them to increase the IP routing table. The figure 288K is taken as a real example from the datasheet of a specific switch to illustrate the order of magnitude.

**Q3:** Why do you say "in 1 nanosecond"? Is this an exact value or an approximation?

**A3:** This is a correct approximation based on the clock frequency of modern TCAM arrays. The key principle is that the search occurs in one clock cycle. If the frequency is 1 GHz, then the cycle duration is 1 nanosecond. Modern chips can operate at even higher frequencies (e.g., 1.5 GHz, which gives ~0.67 ns per operation). Thus, the phrasing "in 1 nanosecond" reflects the real physical speed limit of search in modern network processors.

**Q4:** What exactly happens in that nanosecond? How is it possible to check thousands of entries in time?

**A4:** This is the most important question. The answer lies in the hardware implementation. The check does not happen sequentially but in parallel. All memory cells in a row (constituting one entry) and all rows in the array are engaged simultaneously. When the search data (bitmask) is applied to the search lines (SL), an instantaneous comparison occurs in all cells (thanks to the physical phenomenon of inversion layer formation in transistors, as shown in the GIF). The result — the state of the match line (ML) — manifests itself almost instantly for the entire table. This is not a software algorithm but a physical property of the circuit.

**Q5:** The diagrams show classical CAM, but modern devices use TCAM. What is the difference?

**A5:** Classical CAM searches only for two states: 0 and 1. TCAM (Ternary CAM) adds a third state — "X" (don't care, or mask). This allows for pattern searching. For example, you can find all entries where the first 24 bits (network) match, and the remaining 8 bits (host) are anything. It is TCAM that enables high-speed routing based on long prefixes and the application of complex ACL rules. The principle of parallel search and high speed are fully preserved.