#define WIN32_LEAN_AND_MEAN  /* Exclude rarely-used stuff from Windows headers */
#include <winsock2.h>       /* Include Winsock2 header */
#include <ws2tcpip.h>      /* Include Winsock2 TCP/IP extensions header */
#include <stdio.h>         /* Include standard input/output header */

#pragma comment(lib, "ws2_32.lib") /* Link with the Ws2_32.lib library */

/*
    This structure defines the format of an IPv4 header.  Each field is carefully
    defined to match the expected layout of an IP packet.  Bitfields are used to
    define smaller fields within a byte.

    ihl: Internet Header Length (4 bits).  Specifies the length of the IP header
         in 32-bit words.  The minimum value is 5 (20 bytes).

    version: IP Version (4 bits).  Specifies the IP version number (typically 4 for IPv4).

    tos: Type of Service (8 bits).  Specifies the desired quality of service for
         the packet (e.g., priority, delay, throughput).

    tot_len: Total Length (16 bits).  Specifies the total length of the IP packet
             (header + data) in bytes.

    id: Identification (16 bits).  Used for fragmentation/reassembly of packets.

    frag_off: Fragment Offset (16 bits).  Specifies the offset of the fragment
              relative to the beginning of the original unfragmented IP datagram.

    ttl: Time to Live (8 bits).  Specifies the maximum number of hops the packet
         can take before being discarded.  Decremented by each router.

    protocol: Protocol (8 bits).  Specifies the transport layer protocol used in
              the data portion of the IP packet (e.g., TCP = 6, UDP = 17).

    check: Header Checksum (16 bits).  A checksum of the IP header used for error detection.

    saddr: Source Address (32 bits).  The IPv4 address of the packet's source.

    daddr: Destination Address (32 bits). The IPv4 address of the packet's destination.
*/
typedef struct _iphdr {
    unsigned char  ihl:4;      /* Internet Header Length */
    unsigned char  version:4;  /* IP Version */

    unsigned char  tos;      /* Type of Service */
    unsigned short tot_len;  	/* Total Length */

    unsigned short id ;       	/* Identification */
	unsigned int   frag_off;   /* Fragment Offset */

    unsigned char  ttl;      	/* Time To Live */
    unsigned char  protocol; 	/* Protocol */
    unsigned short check;    	/* Header Checksum */

    unsigned int   saddr;    	/* Source Address */
    unsigned int   daddr;   	/* Destination Address */

} IPHDR, *PIPHDR;

/*
    This structure defines the format of a TCP header.  Like the IP header, each
    field is meticulously defined according to TCP specifications.

    source: Source Port (16 bits).  The port number of the sending application.

    dest: Destination Port (16 bits). The port number of the receiving application.

    seq: Sequence Number (32 bits).  The sequence number of the first byte in the
         TCP segment.  Used for reliable transmission and reassembly.

    ack_seq: Acknowledgment Sequence Number (32 bits).  The sequence number of the
             next byte expected from the receiver.  Used for acknowledging received data.

    res1:4: Reserved (4 bits).  These bits are reserved for future use and must be zero.

    doff:4: Data Offset (4 bits).  Specifies the length of the TCP header in 32-bit words.
           The minimum value is 5 (20 bytes).

    fin:1: FIN Flag (1 bit).  Indicates that the sender has no more data to send.
         Used to terminate the connection.

    syn:1: SYN Flag (1 bit).  Indicates a request to establish a connection.

    rst:1: RST Flag (1 bit).  Indicates that the connection should be reset immediately.

    psh:1: PSH Flag (1 bit).  Indicates that the data should be pushed immediately to the
         receiving application.

    ack:1: ACK Flag (1 bit).  Indicates that the Acknowledgment Sequence Number field is valid.

    urg:1: URG Flag (1 bit).  Indicates that the Urgent Pointer field is valid.

    res2:2: Reserved (2 bits).  These bits are reserved for future use and must be zero.

    window: Window Size (16 bits).  Specifies the amount of data the receiver is
            willing to accept.  Used for flow control.

    check: Checksum (16 bits).  A checksum of the TCP header and data (along with
           a pseudo-header) used for error detection.

    urg_ptr: Urgent Pointer (16 bits).  Specifies the offset from the sequence number
             of the first urgent data byte.  Used when the URG flag is set.
*/
typedef struct _tcphdr {
    unsigned short source;    /* Source Port */
    unsigned short dest;      /* Destination Port */

    unsigned int   seq;       /* Sequence Number */
    unsigned int   ack_seq;   /* Acknowledgment Sequence Number */

    unsigned short res1:4;    /* Reserved */
    unsigned short doff:4;    /* Data Offset */

    unsigned short fin:1;     /* FIN Flag */
    unsigned short syn:1;     /* SYN Flag */
    unsigned short rst:1;     /* RST Flag */
    unsigned short psh:1;     /* PSH Flag */
    unsigned short ack:1;     /* ACK Flag */
    unsigned short urg:1;     /* URG Flag */

    unsigned short res2:2;    /* Reserved */

    unsigned short window;    /* Window Size */
    unsigned short check;     /* Checksum */
    unsigned short urg_ptr;   /* Urgent Pointer */

} TCPHDR, *PTCPHDR;

/*
    This function calculates the checksum of a given buffer.  The checksum is a 16-bit
    value used for error detection in network protocols.  It's crucial for ensuring
    the integrity of IP and TCP headers.

    The algorithm works by summing up all 16-bit words in the buffer.  If the sum
    exceeds 16 bits, the carry is added back to the lower 16 bits.  This process
    is repeated until there are no more carries.  Finally, the 16-bit sum is inverted
    (one's complement) to produce the checksum.

    buf: A pointer to the buffer whose checksum needs to be calculated.
    len: The length of the buffer in bytes.

    returns: The calculated checksum as an unsigned short.
*/
unsigned short checksum(unsigned short *buf, int len) {
    unsigned long sum = 0; /* Initialize sum to 0 */
    while (len > 1) {      /* While there are at least two bytes left */
        sum += *buf++;     /* Add the current 16-bit word to the sum */
        len -= 2;          /* Decrement the length by 2 bytes */
    }
    if (len == 1)       /* If there's a remaining byte */
        sum += *(unsigned char*)buf; /* Add the remaining byte to the sum */

    sum = (sum >> 16) + (sum & 0xFFFF); /* Add the carry (upper 16 bits) to the lower 16 bits */
    sum += (sum >> 16);                 /* Repeat the carry addition if necessary */

    return (unsigned short)(~sum); /* Invert the sum and return the checksum */
}

int main() {
    WSADATA wsaData; /* Structure to hold Windows Sockets startup information */

    /*
        Initializes the Windows Sockets library.  This function must be called before
        any other Winsock functions are used.

        MAKEWORD(2, 2): Specifies the version of Winsock to use (2.2).

        returns: 0 on success, otherwise an error code.
    */
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n"); /* Print error message */
        return 1;                      /* Exit with an error code */
    }

    SOCKET sock; /* Socket descriptor */

    /*
        Creates a socket.  A socket is an endpoint for communication.

        AF_INET: Specifies the IPv4 address family.

        SOCK_RAW: Specifies a raw socket.  Raw sockets allow direct access to the
                  IP protocol, bypassing the normal TCP/UDP processing.  This allows
                  us to craft custom IP packets.  **Requires Administrator Privileges**

        IPPROTO_RAW: Specifies that we're working directly with the IP protocol.

        returns: A socket descriptor on success, INVALID_SOCKET on failure.
    */
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError()); /* Print the error */
        WSACleanup();                    /* Clean up Winsock */
        return 1;                      /* Exit with an error code */
    }

    BOOL opt = TRUE; /* Option to set IP_HDRINCL */

    /*
        Sets socket options.

        sock: The socket descriptor.
        IPPROTO_IP: The protocol level (IP protocol).
        IP_HDRINCL: Option to indicate that we will include the IP header in the
                    data we send.  This gives us full control over the IP header.

        (char*)&opt: A pointer to the option value (TRUE in this case).
        sizeof(opt): The size of the option value.

        returns: 0 on success, SOCKET_ERROR on failure.
    */
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        printf("Setsockopt failed: %d\n", WSAGetLastError()); /* Print the error */
        closesocket(sock);                   /* Close the socket */
        WSACleanup();                    /* Clean up Winsock */
        return 1;                      /* Exit with an error code */
    }

    char packet[4096] = {0}; /* Buffer to hold the IP packet. Initialize to 0 */
    IPHDR *ip = (IPHDR*)packet; /* Pointer to the IP header within the packet */
    TCPHDR *tcp = (TCPHDR*)(packet + sizeof(IPHDR)); /* Pointer to the TCP header */

    /*
        Populating the IP Header Fields

        version: Set the IP version to 4 (IPv4)

        ihl: Set the IP header length to 5 (5 * 4 = 20 bytes, standard IP header size)

        tos: Set the Type of Service to 0 (no priority)

        tot_len: Set the total length of the IP packet (IP header + TCP header).
                 htons converts the short from host byte order to network byte order.

        id: Set an arbitrary ID for the IP packet (used for fragmentation/reassembly).

        frag_off: Set the fragment offset to 0 (no fragmentation)

        ttl: Set the Time To Live to 128 (maximum number of hops).

        protocol: Set the protocol to IPPROTO_TCP (6).

        saddr: Set the source IP address.  **IMPORTANT: Use YOUR actual IP address
               or the packet will likely be dropped by routers.**

        daddr: Set the destination IP address (e.g., example.com).

        check: Initially set to 0, then calculated using the checksum function.
    */
    ip->version = 4;
    ip->ihl = 5;
    ip->tos = 0;
    ip->tot_len = htons(sizeof(IPHDR) + sizeof(TCPHDR));
    ip->id = htons(54321);
    ip->frag_off = 0;
    ip->ttl = 128;
    ip->protocol = IPPROTO_TCP;
    ip->saddr = inet_addr("YOUR_SOURCE_IP_ADDRESS"); /* **REPLACE WITH YOUR ACTUAL IP ADDRESS** */
    ip->daddr = inet_addr("TARGET_DESTINATION_IP");  /* IP for example.com */
    ip->check = 0;
    ip->check = checksum((unsigned short*)ip, sizeof(IPHDR));

    /*
        Populating the TCP Header Fields

        source: Set the source port to 12345 (arbitrary port).

        dest: Set the destination port to 80 (HTTP port).

        seq: Set an arbitrary sequence number.  htonl converts the long from host
             byte order to network byte order.

        ack_seq: Set the acknowledgment sequence number to 0 (since this is a SYN packet).

        doff: Set the data offset to 5 (5 * 4 = 20 bytes, standard TCP header size).

        syn: Set the SYN flag to 1 (this is a SYN packet, connection request).

        window: Set an arbitrary window size.

        check: Initially set to 0, then calculated using the checksum function (including pseudo header).

        urg_ptr: Set the urgent pointer to 0 (no urgent data).
    */
    tcp->source = htons(12345); /* Arbitrary source port */
    tcp->dest = htons(80);       /* HTTP port */
    tcp->seq = htonl(123456789);  /* Arbitrary sequence number */
    tcp->ack_seq = 0;       /* No acknowledgement in SYN packet */
    tcp->doff = 5;          /* TCP header length is 5 * 4 = 20 bytes */
    tcp->syn = 1;           /* SYN flag set to 1 */
    tcp->window = htons(5840); /* Arbitrary window size */
    tcp->check = 0;         /* Calculate later */
    tcp->urg_ptr = 0;       /* No urgent pointer */

    /*
        The TCP checksum calculation requires a "pseudo-header" that includes parts of
        the IP header. This structure is used for the checksum calculation only and is
        not sent over the network.
    */
    struct {
        unsigned int src_addr;   /* Source Address */
        unsigned int dst_addr;   /* Destination Address */
        unsigned char zero;       /* Zero byte */
        unsigned char protocol;   /* Protocol (TCP) */
        unsigned short length;  /* TCP Length */
    } pseudo_header;

    pseudo_header.src_addr = ip->saddr;
    pseudo_header.dst_addr = ip->daddr;
    pseudo_header.zero = 0;
    pseudo_header.protocol = IPPROTO_TCP;
    pseudo_header.length = htons(sizeof(TCPHDR)); /* TCP header length */

    /*
        Creates a buffer to hold the pseudo-header and the TCP header for checksum calculation.
    */
    char tcp_packet[sizeof(pseudo_header) + sizeof(TCPHDR)];

    /*
        Copies the pseudo-header and TCP header into the tcp_packet buffer.
    */
    memcpy(tcp_packet, &pseudo_header, sizeof(pseudo_header));
    memcpy(tcp_packet + sizeof(pseudo_header), tcp, sizeof(TCPHDR));

    /*
        Calculates the TCP checksum.  The checksum covers the pseudo-header and the TCP header.
    */
    tcp->check = checksum((unsigned short*)tcp_packet, sizeof(tcp_packet));

    struct sockaddr_in dest_addr; /* Structure for the destination address */
    dest_addr.sin_family = AF_INET; /* Address family is IPv4 */
    dest_addr.sin_port = htons(80);    /* Destination port */
    dest_addr.sin_addr.s_addr = ip->daddr; /* Destination IP address */

    /*
        Sends the IP packet.

        sock: The socket descriptor.
        packet: The buffer containing the IP packet (IP header + TCP header).
        ntohs(ip->tot_len): The total length of the IP packet.
        0: Flags (no flags used).
        (struct sockaddr*)&dest_addr: Pointer to the destination address structure.
        sizeof(dest_addr): The size of the destination address structure.

        returns: The number of bytes sent on success, SOCKET_ERROR on failure.
    */
    if (sendto(sock, packet, ntohs(ip->tot_len), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError()); /* Print the error */
    } else {
        printf("TCP SYN packet sent successfully!\n");  /* Success message */
    }

    closesocket(sock); /* Close the socket */
    WSACleanup();    /* Clean up Winsock */
    return 0;          /* Exit successfully */
}