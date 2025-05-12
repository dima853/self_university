https://files.arav.su/file/books/computers%20and%20technology/Networking/TCP_IP%20Illustrated.pdf

# **TCP/IP SYN implementation (conceptually)**

---

#### **1. Creating a raw socket**
- **Goal**: To gain access to the network stack for manual packet generation.
- **Features**:
- The socket type `SOCK_RAW` is used (allows you to set headers manually).
  - Windows requires `IPPROTO_RAW', Linux requires `IPPROTO_TCP'.
  - You need administrator rights (root/sudo).

---

#### **2. Formation of the IP header**
- **Fields**:
- `version=4', `ihl=5` (IPv4, header length 20 bytes).
  - `tot_len': Total packet size (IP + TCP headers).
  - `TTL': Lifetime (usually 64).
  - `protocol=6` (TCP).
  - `saddr`, `daddr': The IP of the sender and recipient.
- **Checksum**:
- Calculated for the IP header using the algorithm:
    1. Split the header into 16-bit words.
    2. Summarize all the words.
    3. Add hyphenations (if the sum exceeds 16 bits).
    4. Invert the result bits.

---

#### **3. TCP header generation**
- **Fields**:
- `source`, `dest': Sender and recipient ports.
  - `seq`: The initial sequence number (usually random).
  - `ack_seq=0' (for the SYN packet).
  - `doff=5` (header length in 32-bit words).
  - Flags: `SYN=1', others (`ACK`, `FIN`, etc.) = 0.
  - `window': Window size (for example, 5840).
- **Checksum**:
- Includes:
    1. Pseudo header (fields from IP: `saddr`, `daddr`, `protocol`, `tcp_length').
    2. TCP header.
    3. Data (there is no data for the SYN packet).
  - The calculation algorithm is similar to IP.

---

#### **4. Package Assembly**
- **Order**:
1. Allocate a buffer (for example, 4096 bytes).
2. Write the IP header to the beginning of the buffer.
3. Write the TCP header immediately after the IP.
  4. Fill in the checksums.

---

#### **5. Sending a package**
- **Method**: `sendto()'.
- **Parameters**:
- Socket.
  - Data buffer.
  - Destination address (`struct sockaddr_in` with IP and port).
- **Features**:
- In Windows, you need to specify `IP_HDRINCL` (the header is included in the data).
  - On Linux, the package is sent "as is".

---

#### **6. Response processing (optional)**
- **Expected response**: a `SYN-ACK` packet from the server.
- **Fields to check**:
- `ack_seq = source seq + 1'.
  - Flags: `SYN=1', `ACK=1'.

---

#### **Key difficulties**
1. **Byte order**: All numeric fields are in network order (big-endian, `htons/htonl`).
2. **Alignment of structures**: In C/C++, bit fields may depend on the compiler.
3. **Security**: Raw sockets require privileges and can be blocked by a firewall.
