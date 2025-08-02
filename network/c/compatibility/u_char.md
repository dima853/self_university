### **Why is `u_char` used for Ethernet headers (and not `unsigned char')?**

In network programming, especially in code related to **Ethernet**, **TCP/IP** and **low-level packets**, `u_char` is often found. Here are the main reasons:

---

## **1. Historical reasons (UNIX/Linux legacy)**
- Older UNIX systems (BSD, Solaris) used **abbreviated aliases** for data types:
- `u_char` → `unsigned char`
  - `u_short` → `unsigned short`
  - `u_int` → `unsigned int`
  - `u_long` → `unsigned long`
- These aliases were introduced to **simplify the code** in the kernel and drivers.
- **Ethernet headers** and **network structures** were defined back in the 1980s, and their API remained backward compatible.

---

## **2. The conventions in network APIs**
- **`<netinet/if_ether.h>`** (Ethernet header) and **`<netinet/ip.h>`** (IP headers) use `u_char` because:
  - They appeared in **BSD** (Berkeley Software Distribution), where `u_char` was the standard.
  - Linux inherited these headers for compatibility.
- Example from `if_ether.h`:
``c
struct ethhdr {
      u_char h_dest[ETH_ALEN]; // Destination MAC address
      u_char h_source[ETH_ALEN]; // MAC address of the source
      u_short h_proto; // Protocol type (IPv4/IPv6)
};
  ```
  Here `u_char` is used for MAC addresses (6 bytes).

---

## **3. Why not `unsigned char`?**
- In **network programming**, `u_char` emphasizes that these are **raw bytes**, not characters.
- This is **documentation code**: when a developer sees `u_char`, he immediately realizes that this is **binary data**, not text.
- In the Linux kernel and drivers, `u_char` is more common than `unsigned char'.

---

## **4. Modern alternatives**
Today, instead of `u_char`, you can use:
1. **`uint8_t`** (from `<stdint.h>`) — explicitly indicates an 8-bit number.
   ``c
uint8_t mac_addr[6]; // Modern style
   ```
2. **`unsigned char`** — if portability is important.

But **network structures** (`ethhdr`, `iphdr', `tcphdr') still use `u_char` for compatibility.

---

## **5. Conclusion**
| Type                | Where it is used                                    | Recommendation                                               |
| ------------------- | --------------------------------------------------- | ------------------------------------------------------------ |
| **`u_char`**        | Old UNIX code, network headers (`<netinet/*.h>`)    | Use if you are working with `libpcap`, `eBPF`, `raw sockets' |
| **`unsigned char`** | Standard C, cross-platform code                     | Better for new code                                          |
| **`uint8_t`**       | Modern C (C99+), where precise bitness is important | Ideal for new projects                                       |

### **Result**
The 'u_char` in Ethernet headers is **legacy UNIX**, but it is still used for compatibility.  
For new code, it is better to use `uint8_t` or `unsigned char'.