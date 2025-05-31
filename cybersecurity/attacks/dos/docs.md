---

## **1. Checksum — "Packet fingerprint"**
**Why?**
Imagine you're sending a letter by mail. To make sure no one has damaged it along the way, you write in the corner:
*"This letter has exactly 1000 characters"*

If the postman delivers the letter and it has 999 characters, something has been lost.

**It's the same on the network:**
- When a computer sends an IP packet, it **calculates the checksum** — it's like a "digital signature" of the packet.
- If the packet is damaged along the way (due to interference, routing errors), the recipient **recalculates the sum** and realizes that the data is broken.
- Such a packet will simply be thrown away.

**In the code, these are the lines:**
```c
ip->check = 0; // Zero before calculation
ip->check = csum((unsigned short*)ip, sizeof(struct iphdr)); // Calculate the sum for IP
tcp->check = csum((unsigned short*)tcp, sizeof(struct tcphdr)); // And for TCP
```

**If there were no checksum:**
- Packets could lose data, and the recipient would not notice the error.
- Attackers could **deliberately corrupt packets** (for example, spoof addresses).

---

## **2. TTL (Time To Live) — "Packet expiration date"**
**Why?**
Imagine that you are launching a paper airplane around the office. To prevent it from flying forever, you write on it:
*"Maximum 5 throws, then throw away"*

**On the network:**
- Each time a packet passes through a router, the TTL **decreases by 1**.
- If the TTL drops to **0**, the packet is destroyed.

**In code:**
```c
ip->ttl = 255; // Normal value for Linux/Windows
```

**Why is this in SYN flood?**
- To prevent packets from clogging the network if the server does not accept them.
- Without TTL, "garbage" packets could wander around the Internet forever.

---

## **3. IP Spoofing (ip->saddr = rand()) — "Sender Masking"**
**Why?**
Imagine you're calling someone and **replacing the number** with a random one.

**In SYN flood:**
- The sender's real IP is replaced with a **random** one.
- The server tries to respond to the SYN-ACK, but... there is no such address.
- As a result, the connection "hangs" in a half-open state until the timeout expires.

**In code:**
```c
ip->saddr = htonl(rand()); // Substituting the original IP
```

**Why is this important for the attack?**
- If the attacker used a **real IP**, the server could:
- Block it with a firewall.
- Send an RST (reset connection).
- With fake IPs, the attack is harder to stop.

---

## **4. Why is the SYN flag important in SYN flood?**
**How ​​a normal connection (3-way handshake) works:**
1. Client → Server: **SYN** ("Let's connect?")
2. Server → Client: **SYN-ACK** ("OK, ready")
3. Client → Server: **ACK** ("Great, let's start!")

**How ​​SYN flood works:**
- The attacker sends **only SYN**, but does not respond to SYN-ACK.
- The server waits for a response and **keeps the connection in memory**.
- If there are thousands of such "half-open" connections, the server **will run out of memory** and crash.

**In the code it is:**
```c
tcp->syn = 1; // Set only the SYN flag
tcp->ack = 0; // No ACK (default 0)
```

---

## **How ​​can I simplify the code?**
```python
while True:
# 1. Create a "fake" IP packet
ip_packet = IP(src=random_ip(), dst=target_ip)

# 2. Add TCP with SYN flag
tcp_packet = TCP(sport=random_port(), dport=target_port, flags="S")

# 3. Send and repeat
send(ip_packet / tcp_packet)
time.sleep(0.01)
```

---

## **Output**
- **Checksum** — so that the packet is not damaged along the way.
- **TTL** — so that the packets do not wander around the network forever.
- **IP spoofing** — so that the server cannot track the attacker.
- **SYN flag** — so that the server wastes resources on "half-open" connections.

It's like **"loading the mailman with a million letters from fake addresses"** — sooner or later the mail will stop coping.

---

### **What will happen if you run this on your server?**

1. **The network interface** will start to overload.

2. Thousands of `SYN_RECV` will appear in `netstat -tulnp`.
3. If the server is weak, it will **stop responding** to new connections.

**How ​​to protect yourself?**
- Enable **SYN cookies** (`echo 1 > /proc/sys/net/ipv4/tcp_syncookies`).
- Set up a **firewall** (e.g. `iptables`).
- Use **Cloudflare** or other DDoS protection.