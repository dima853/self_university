---

# **1. Physical structure of DDR4 (BGA enclosure)**
DDR4 memory is available in **BGA enclosures** (Ball Grid Array), where each "ball" is a contact for signal, power or ground.  

![alt text](image.png)

### **1.1. The location of the balls (Balloon)**
- **x4/x8 chips**: **13 electrical rows** (rows).  
- **x16 chips**: **16 electrical rows**.  
- **There may be non-electric balls between them** (for mechanical support).  

#### **Example for x8 DDR4 (13 electrical rows):**
```
Row 0:  VDD  DQ0  DQ1  VSS  DQ2  DQ3  ...  
Row 1:  A0   A1   BA0  VDDQ A2   A3   ...  
...  
Row 12: CK_t CK_c VSS VDD WE CAS ...
` `
(This is a simplified example, the actual wiring is more complicated.)*  

### **1.2. Pitch between balls**
- **Pitch**: **0.8 mm × 0.8 mm** (distance between the centers of the balls).  
- **Depopulated columns**: **3 empty columns** between active ones.  

#### **Why depopulated columns?**  
1. **Reduction of crosstalk** (less mutual influence of signals).  
2. **Easier wiring of PCB** (memory cards).  
3. **Mechanical stability** (less risk of heat damage).  

---

# **2. Electric vs Mechanical balls**
- **Electric balls**: Transmit signals, power (VDD), ground (VSS).  
- **Mechanical balls**: Not connected to anything, only for the rigidity of the body.  

#### **Example in C (abstract BGA model):**
``c
#include <stdint.h>

// Ball type
typedef enum {
    BALL_TYPE_GROUND,   // VSS
    BALL_TYPE_POWER,    // VDD
    BALL_TYPE_SIGNAL, // DQ0, A0, CK_t, etc.
    BALL_TYPE_NC // Not connected (mechanical)
} BallType;

// Description of one balloon
typedef struct {
    uint8_t row; // Row number (0-12 for x8)
uint8_t column; // Column number
    BallType type; // Type
const char *name; // Name of the signal (for example, "DQ0")
} BGABall;

// Example of a BGA description for DDR4 x8
BGABall ddr4_x8_bga[] = {
    {0, 0, BALL_TYPE_POWER, "VDD"},
    {0, 1, BALL_TYPE_SIGNAL, "DQ0"},
    {0, 2, BALL_TYPE_SIGNAL, "DQ1"},
    {0, 3, BALL_TYPE_GROUND, "VSS"},
    {0, 4, BALL_TYPE_SIGNAL, "DQ2"},
    {0, 5, BALL_TYPE_NC, "NC"}, // Empty ball
// ... and so on for all rows
};
``

---

# **3. Why is all this necessary?**
### **3.1. Signal groups in DDR4**
1. **Address lines (A0-A17)**: Bank/row/column selection.  
2. **Data (DQ0-DQ63)**: Data transmission (depends on bus width).  
3. **Control signals (CK_t, CK_c, WE, CAS, RAS)**: Synchronization and commands.  
4. **Power Supply (VDD, VDDQ, VSS)**: Different voltages for core and I/O.  

#### **Example in C: Reading from DDR4 (imitation)**
``c
// Let's assume that we have a function for accessing BGA contacts
void ddr4_send_signal(const char *signal_name, uint32_t value) {
    printf("[DDR4] Set %s = %u\n", signal_name, value);
}

// Reading data from DDR4 (imitation)
uint32_t ddr4_read(uint32_t bank, uint32_t row, uint32_t col) {
    // 1. Activate the line (ACTIVATE)
ddr4_send_signal("A0-A17", row); // Set the address of the line
    ddr4_send_signal("BA0-BA2", bank); // Select a bank
    ddr4_send_signal("RAS", 0); // RAS=0 (string activation)

    // 2. Waiting for tRCD (RAS-to-CAS delay)
delay_nanoseconds(15); // Example: 15 ns

    // 3. Read the column (READ)
ddr4_send_signal("A0-A10", col); // Column address
    ddr4_send_signal("CAS", 0); // CAS=0 (read)

    // 4. Waiting for CL (CAS Latency)
delay_nanoseconds(16); // Example: CL=16 clock cycles

    // 5. We accept data from DQ0-DQn
    uint32_t data = 0;
    for (int i = 0; i < 32; i++) {
data |= (ddr4_read_dq(i) << i); // Reading every bit
    }

    // 6. Close the bank (PRECHARGE)
ddr4_send_signal("RAS", 1);
    ddr4_send_signal("WE", 1);

    return data;
}
``
*(In reality, this is done by the memory controller, but this is how the logic can be modeled.)*  

---

# **4. The difference between x4, x8 and x16**
- **x4**: 4-bit data bus (rare, usually for ECC).  
- **x8**: 8-bit (the most common version).  
- **x16**: 16-bit (for cheaper, but smaller banks).  

#### **Example: Organization of chips on a DIMM module**
- **Regular DDR4-3200 16 GB DIMM**:
- 8 x8 chips (8 bits × 8 chips = 64 bits).  
  - Each chip has **16 banks**.  
- **ECC module**:
- 9 x8 chips (64 bits + 8 bits ECC).  

---

# **5. How does this relate to programming?**
### **5.1. Memory access in C**
Although BGA balls cannot be directly controlled, embedded systems can work with the registers of the memory controller.:  
``c
// Example for ARM (abstract)
volatile uint32_t *ddr4_ctrl = (uint32_t*)0xE0000000; // Controller address

void ddr4_init() {
ddr4_ctrl[0x00] = 0x1; // Enabling DDR4
    ddr4_ctrl[0x04] = 0x3A280; // Setting timings (tCL=14, tRCD=14)
ddr4_ctrl[0x08] = 0x1000; // Setting banks
}
``

### **5.2. Optimizing the code for DDR4**
To use DDR4 efficiently:  
1. **Data alignment** (`alignas(64)` for cache lines).  
2. **Batch read/write** (using burst mode DDR4).  
3. **Avoiding frequent transfers between banks** (bank conflicts).  

```c
#include <stdalign.h>

// Cache line alignment (usually 64 bytes)
alignas(64) uint8_t buffer[1024];

// Batch filling (better for DDR4)
void fill_buffer() {
    for (int i = 0; i < 1024; i += 8) {
        // Writing 8 bytes in one burst
*(uint64_t*)(buffer + i) = 0x1122334455667788;
    }
}
```

---

# **6. Useful materials**
1. **JEDEC DDR4 Standard** ([JESD79-4](https://www.jedec.org/standards-documents/docs/jesd79-4 )) — Official document.
2. **Micron DDR4 Dataset** ([PDF](https://www.micron.com/-/media/client/global/documents/products/data-sheet/dram/ddr4/8gb_ddr4_sdram.pdf)) — real BGA templates.  
3. **BGA Routing Guidelines** ([Altera](https://www.intel.com/content/www/us/en/docs/programmable/683082/21-3/pcb-layout-guidelines.html) ) — how to breed DDR4 on the board.  

---

# **Conclusion**
- **BGA DDR4** is a complex grid of balls, where **13 (x4/x8) or 16 (x16) rows** are used for signals, and the rest for mechanics.  
- **Depopulated columns** are needed to reduce interference and simplify wiring.  
- **x4/x8/x16** is the organization of the data bus inside the chip.  
- In real code **, access to DDR4** goes through the memory controller, but understanding the physical structure helps in optimization.