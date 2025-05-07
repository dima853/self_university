# Additional materials
- https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf

# Architecture UPD/53
### **UDP/53 architecture (DNS over UDP) - Clean structure without code**  

#### **1. The general algorithm of work**  
1. **The client** generates a DNS query (binary packet according to RFC 1035).
2. **Sending** via a UDP socket to port 53 of the DNS server.  
3. **Server** processes the request and returns a response.
4. **The client** parses the response and extracts the necessary data (IP, CNAME, etc.).  

---

#### **2. System components**  

**1) Network Layer (UDP):**  
- Creating a socket (`socket()`).
- Binding to a port (optional for the client).  
- Setting timeouts ('setsockopt()').  

**2) DNS query generation:**  
- **Headline:**
- `ID' (unique identifier).  
  - `Flags' (RD=1 for recursion, Opcode, other flags).  
  - 'QDCOUNT = 1' (one question).  
- **Question Section:**
- 'QNAME' (domain in a special format, e.g. `3www6google3com0`).  
  - 'QTYPE' (A, AAAA, MX, etc.).
- `QCLASS' (usually `IN`).  

**3) Sending/receiving data:**  
- `sendto()` — sending a request.  
- `recvfrom()' — receiving a response (max 512 bytes for UDP).  

**4) Response parsing:**  
- Checking the `ID' (must match the request).  
- RCODE analysis (0 = OK, 3 = NXDOMAIN, etc.).
- Data extraction from sections:  
  - `Answer' (IP address).  
  - `Authority' (name servers).  
  - `Additional` (additional data).  

**5) Error handling:**  
- Timeouts (`recvfrom()` with `SO_RCVTIMEO').  
- Check the `TC=1` flag (TCP is needed for large responses).  
- Repeat requests if necessary.  

---

#### **3. Interaction scheme**  
```
Client:  
1. socket() → 2. Generates a DNS query → 3. sendto()
4. recvfrom() → 5. Parses the response → 6. close()  

The server:  
1. Accepts the request → 2. Searches the DATABASE/cache → 3. Sends the response  
```

---

#### **4. Features of UDP/53**  
- **Max. packet size:** 512 bytes (if larger, `TC=1` and switch to TCP).  
- **Recursion:** If the server does not support (RD=0), the client makes the requests itself.  
- **Caching:** Local cache to speed up repeated requests.  

---

#### **5. What is not included in the architecture (but important)**  
- Support for DNSSEC (data signing).  
- IPv6 (AAAA records).  
- Asynchronous requests (via `select()`/`poll()`).  

---

This framework describes the **basic framework** for implementing a DNS client. For the server, we add:
- A database of records (or a link to higher-level DNS).  
- Processing recursive queries.