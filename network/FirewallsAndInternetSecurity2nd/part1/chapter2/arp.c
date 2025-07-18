#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <net/if.h>

/*
## **1. Two-step address resolution process**
### **The essence:**
ARP implements a request-response mechanism for dynamically mapping IP addresses (logical) to MAC addresses (physical) in a local network (L2 segment).

### **Details:**
1. **Broadcast Request (ARP Request)**:
- A node that needs to find out the MAC address using a known IP sends a **broadcast packet** (to MAC `FF:FF:FF:FF:FF:FF`).
   - The request specifies:
     - Your IP and MAC (fields `sender_ip/sender_mac`).
     - Target IP (`target_ip'), but leaves the target's MAC blank (`00:00:00:00:00:00`).

2. **Unique Response (ARP Reply)**:
- Only the node with the requested IP responds with a **unicast packet** by filling in the 'target_mac` field with its MAC address.
   - The other nodes ignore the request if their IP does not match the `target_ip'.

### **Example scenario**:
- Host A (`IP_A`, `MAC_A`) wants to send data to host B (`IP_B`, but unknown to `MAC_B`).
- A sends an ARP Request with `target_ip = IP_B'.
- B responds with ARP Reply with `sender_mac = MAC_B'.
- A caches the pair `IP_B → MAC_B` in its ARP table.
*/

/*

## **2. What does an ARP packet look like?*
An ARP packet is a **data structure** that contains:

### **Ethernet header (L2)**
| Field | Value | Description |
|----------------|-----------------------------|-----------------------------------|
| `h_dest`       | `FF:FF:FF:FF:FF:FF` | Broadcast (to everyone online)           |
| `h_source`     | `your MAC` | sender's MAC |
| `h_proto`      | `0x0806` | Protocol type (ARP) |

### **ARP header**
| Field | Value | Description |
|-------------------|-----------------------------|-----------------------------------|
| `hardware_type` | `1` (Ethernet) | Network type (Ethernet = 1)           |
| `protocol_type` | `0x0800` (IPv4) | Which protocol are we looking for (IPv4) |
| `hardware_len`    | `6` | Length of the MAC address (6 bytes)         |
| `protocol_len`    | `4` | IPv4 address length (4 bytes)       |
| `opcode` | `1' (ARP request)            | `1` = request, `2` = response |
| `sender_mac`     | `your MAC` | Who's asking |
| `sender_ip`      | `your IP` | The IP of the person who is asking |
| `target_mac`     | `00:00:00:00:00:00` | Unknown yet (will fill in the response) |
| `target_ip`      | `192.168.1.1` | The IP whose MAC we are looking for |

*/
void send_arp_request(const char *interface, const char *target_ip_str)
{
    int sockfd;
    struct sockaddr_ll socket_address;
    unsigned char buffer[ETH_FRAME_LEN];
    struct ethhdr *eth = (struct ethhdr *)buffer;
    struct arp_header *arp = (struct arp_header *)(buffer + sizeof(struct ethhdr));

    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) // indicates that the socket will receive all types of Ethernet packets.
    {
        fprintf(stderr, "Error when creating a socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface, IFNAMSIZ);

    /**
     * ioctl may return the following errors (errno):
     *
     * EBADF:
     * fd is not a valid file descriptor.  This means that either the socket
     * was not created, or was closed before the ioctl call.
     *
     * EFAULT:
     * argp - indicates an inaccessible memory area. There is probably a problem with the address of the ifr structure.
     *
     * EINVAL:
     * The query or argp is not valid. This means that either SIOCGIFINDEX is not
     * an ioctl-supported query, or the ifr structure is filled in incorrectly.
     *
     * ENOTTY:
     * fd is not associated with a character device.  And also that the specified operation does not apply to the type
     * the object referenced by the fd file descriptor. In our case, most likely,
     * The specified interface does not exist or is inactive.
     *
     * Other errors may occur depending on the specific system and drivers.
     */
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0)
    {
        fprintf(stderr, "Error when calling ioctl for the interface '%s': %s\n", interface_name, strerror(errno));
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    /**
     * @brief Initializes the sockaddr_ll structure for sending raw Ethernet packets.
     *
     * This section of code prepares the `sockaddr_ll` structure, which is essential
     * for sending packets directly over a network interface using a raw socket. It sets
     * various fields to configure the socket address, including the family, protocol,
     * interface index, hardware type, packet type, and address length.
     *
     * @param socket_address A pointer to the `sockaddr_ll` structure to be initialized.
     * @param ifr The `ifreq` structure containing information about the network interface,
     *            specifically the interface index.
     *
     * @details
     * - `memset(&socket_address, 0, sizeof(socket_address));`: Zeroes out the entire
     *   `socket_address` structure to ensure no garbage data is present. This is a
     *   common practice to avoid unexpected behavior due to uninitialized memory.
     *
     * - `socket_address.sll_family = AF_PACKET;`: Sets the address family to `AF_PACKET`,
     *   indicating that this socket will be used for raw packet access.  This is
     *   different from `AF_INET` (for IPv4) or `AF_INET6` (for IPv6).
     *
     * - `socket_address.sll_protocol = htons(ETH_P_ARP);`: Sets the protocol to `ETH_P_ARP`,
     *   specifying that this socket will be used to send and receive ARP (Address Resolution Protocol)
     *   packets. `htons()` converts the protocol value from host byte order to network byte order.
     *   Other common protocol values include `ETH_P_IP` for IPv4 packets and `ETH_P_IPV6` for IPv6 packets.
     *
     * - `socket_address.sll_ifindex = ifr.ifr_ifindex;`: Sets the interface index to the
     *   value obtained from the `ifr` structure. The interface index identifies the specific
     *   network interface to use for sending and receiving packets.  This value is crucial for
     *   binding the socket to a particular interface.
     *
     * - `socket_address.sll_hatype = htons(ARPHRD_ETHER);`: Sets the hardware address type to
     *   `ARPHRD_ETHER`, indicating that we are working with Ethernet frames.  `ARPHRD_ETHER`
     *   is a standard constant representing Ethernet hardware. Again, `htons()` ensures correct byte order.
     *
     * - `socket_address.sll_pkttype = PACKET_BROADCAST;`: Sets the packet type to `PACKET_BROADCAST`,
     *   specifying that we intend to send broadcast packets.  Other possible values include
     *   `PACKET_HOST` (for packets destined for the local host), `PACKET_MULTICAST` (for multicast
     *   packets), and `PACKET_OTHERHOST` (for packets destined for a different host).
     *
     * - `socket_address.sll_halen = ETH_ALEN;`: Sets the hardware address length to `ETH_ALEN`, which
     *   is the standard length of an Ethernet MAC address (6 bytes).  This field indicates the length
     *   of the hardware address that will be used in the `sll_addr` field (not shown in this snippet).
     */
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sll_family = AF_PACKET;
    socket_address.sll_protocol = htons(ETH_P_ARP);
    socket_address.sll_ifindex = ifr.ifr_ifindex;
    socket_address.sll_hatype = htons(ARPHRD_ETHER);
    socket_address.sll_pkttype = PACKET_BROADCAST;
    socket_address.sll_halen = ETH_ALEN;

    /**
     * @brief  Constructs an Ethernet frame and an ARP (Address Resolution Protocol) request.
     *
     * This code snippet prepares the Ethernet frame header and the ARP payload
     * for an ARP request.  An ARP request is a broadcast message used to discover
     * the MAC address associated with a given IP address.
     *
     * @param eth  A pointer to an `ethhdr` structure, which will be filled with Ethernet header information.
     * @param arp  A pointer to an `arphdr` structure, which will be filled with ARP payload information.
     * @param sockfd  The socket file descriptor, used for ioctl calls.
     * @param ifr  The `ifreq` structure containing interface information, including the MAC address.
     *
     * @details
     * - `memset(eth->h_dest, 0xff, ETH_ALEN);`: Sets the destination MAC address in the
     *   Ethernet header to the broadcast address (FF:FF:FF:FF:FF:FF).  This ensures that
     *   the ARP request is sent to all devices on the local network segment.
     *
     * - `if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) { ... }`: Calls `ioctl` with
     *   `SIOCGIFHWADDR` to retrieve the hardware (MAC) address of the network interface.
     *   This is the source MAC address for our Ethernet frame.
     *   - `perror("ioctl");`: Prints an error message to `stderr` if `ioctl` fails.  This is
     *     critical for debugging.
     *   - `close(sockfd); exit(EXIT_FAILURE);`:  Closes the socket and exits the program on error.
     *
     * - `memcpy(eth->h_source, ifr.ifr_hwaddr.sa_data, ETH_ALEN);`: Copies the interface's MAC address
     *   from the `ifr` structure into the source MAC address field of the Ethernet header.
     *
     * - `eth->h_proto = htons(ETH_P_ARP);`: Sets the Ethernet protocol type to `ETH_P_ARP`,
     *   indicating that the payload of this Ethernet frame is an ARP packet.  `htons` converts
     *   from host to network byte order.
     *
     * - `arp->hardware_type = htons(ARPHRD_ETHER);`: Sets the ARP hardware type to `ARPHRD_ETHER`,
     *   specifying that we are using Ethernet.
     *
     * - `arp->protocol_type = htons(ETH_P_IP);`: Sets the ARP protocol type to IPv4.
     *
     * - `arp->hardware_len = ETH_ALEN;`: Sets the hardware address length to the length of
     *   an Ethernet MAC address (6 bytes).
     *
     * - `arp->protocol_len = 4;`: Sets the protocol address length to 4, which is the
     *   length of an IPv4 address (in bytes).
     *
     * - `arp->opcode = htons(ARPOP_REQUEST);`: Sets the ARP opcode to `ARPOP_REQUEST`,
     *   indicating that this is an ARP request message. Other possible opcodes include `ARPOP_REPLY`.
     */
    memset(eth->h_dest, 0xff, ETH_ALEN); // Broadcast MAC
    // Получаем MAC интерфейса
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0)
    {
        perror("ioctl");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    memcpy(eth->h_source, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
    eth->h_proto = htons(ETH_P_ARP);

    // Заполняем ARP заголовок
    arp->hardware_type = htons(ARPHRD_ETHER);
    arp->protocol_type = htons(ETH_P_IP);
    arp->hardware_len = ETH_ALEN;
    arp->protocol_len = 4;
    arp->opcode = htons(ARPOP_REQUEST);

    /**
     * @brief  Completes the ARP request by filling in the sender and target information,
     *         and then sends the constructed packet over the raw socket.
     *
     * This code snippet populates the remaining fields of the ARP request, including
     * the sender's MAC and IP addresses, and the target's IP address. It then sends
     * the complete Ethernet frame containing the ARP request out onto the network.
     *
     * @param arp  A pointer to the `arphdr` structure (ARP header) to be filled.
     * @param ifr  A pointer to the `ifreq` structure containing interface information
     *             (MAC and IP addresses).
     * @param sockfd  The socket file descriptor.
     * @param target_ip_str A string containing the target IP address (e.g., "192.168.1.1").
     * @param buffer  A pointer to the buffer containing the entire Ethernet frame (header + ARP payload).
     * @param socket_address A pointer to the `sockaddr_ll` structure containing socket address information.
     *
     * @details
     * - `memcpy(arp->sender_mac, ifr.ifr_hwaddr.sa_data, ETH_ALEN);`: Copies the interface's
     *   MAC address from the `ifr` structure to the sender's MAC address field in the ARP header.
     *   This is the MAC address of the device sending the ARP request.
     *
     * - `if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0) { ... }`: Uses `ioctl` with `SIOCGIFADDR`
     *   to retrieve the IP address of the network interface.
     *   - `perror("ioctl");`: Prints an error message if `ioctl` fails.
     *   - `close(sockfd); exit(EXIT_FAILURE);`: Closes the socket and exits if there's an error.
     *
     * - `struct sockaddr_in *ipaddr = (struct sockaddr_in *)&ifr.ifr_addr;`: Casts the
     *   generic socket address (`ifr.ifr_addr`) to a `sockaddr_in` structure pointer to
     *   access the IPv4 address.
     *
     * - `memcpy(arp->sender_ip, &ipaddr->sin_addr, 4);`: Copies the IPv4 address from the
     *   `sockaddr_in` structure to the sender's IP address field in the ARP header. This is
     *   the IP address of the device sending the ARP request.
     *
     * - `memset(arp->target_mac, 0x00, ETH_ALEN);`: Sets the target MAC address in the
     *   ARP header to all zeros. This is because we don't know the target MAC address;
     *   that's what we're trying to find with the ARP request.
     *
     * - `struct in_addr target_ip; inet_pton(AF_INET, target_ip_str, &target_ip);`: Converts
     *   the string representation of the target IP address (e.g., "192.168.1.1") to a
     *   binary IPv4 address using `inet_pton`.
     *
     * - `memcpy(arp->target_ip, &target_ip, 4);`: Copies the binary IPv4 address to the target IP
     *   address field in the ARP header. This is the IP address we're trying to resolve to a MAC address.
     *
     * - `if (sendto(sockfd, buffer, sizeof(struct ethhdr) + sizeof(struct arp_header), 0,
     *       (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0) { ... }`:
     *   Sends the Ethernet frame (containing the ARP request) over the raw socket using `sendto`.
     *   - `sockfd`: The socket file descriptor.
     *   - `buffer`: The buffer containing the complete Ethernet frame.
     *   - `sizeof(struct ethhdr) + sizeof(struct arp_header)`: The size of the data to be sent (Ethernet header + ARP payload).
     *   - `0`: Flags (usually 0 for standard sending).
     *   - `(struct sockaddr *)&socket_address`: A pointer to the `sockaddr_ll` structure, specifying
     *     the destination (the network interface).
     *   - `sizeof(socket_address)`: The size of the `sockaddr_ll` structure.
     *   - `perror("sendto");`: Prints an error message if `sendto` fails.
     *
     * - `close(sockfd);`: Closes the socket after sending the packet.
     */
    memcpy(arp->sender_mac, ifr.ifr_hwaddr.sa_data, ETH_ALEN);

    if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0)
    {
        perror("ioctl");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in *ipaddr = (struct sockaddr_in *)&ifr.ifr_addr;
    memcpy(arp->sender_ip, &ipaddr->sin_addr, 4);

    memset(arp->target_mac, 0x00, ETH_ALEN);
    struct in_addr target_ip;
    inet_pton(AF_INET, target_ip_str, &target_ip);
    memcpy(arp->target_ip, &target_ip, 4);

    if (sendto(sockfd, buffer, sizeof(struct ethhdr) + sizeof(struct arp_header),
               0, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0)
    {
        perror("sendto");
    }

    close(sockfd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Использование: %s <интерфейс> <целевой IP>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    send_arp_request(argv[1], argv[2]);
    return 0;
}