#include <stdio.h> // For printf()
#include <stdint.h> // For uint8_t, uint16_t and other fixed size types
#include <string.h> // For memcpy(), memset()
#include <unistd.h> // For close()
#include <sys/socket.h> // For socket()
#include <netinet/ether.h> // For ETH_P_ALL
#include <linux/if_packet.h> // For sockaddr_ll
#include <net/if.h> // For if_nametoindex()

/*
  @Preamble and @SFD are not included in the header (the network card handles them)
*/
typedef struct {
  uint8_t dest_mac[6];
  uint8_t src_mac[6];
  uint16_t ethertype;
  uint8_t payload[1500];
  uint32_t crc;
} EthernetFrame;

/*
 @
*/
void print_mac(const uint8_t mac[6], size_t size) {
    if (size != 6) {
        fprintf(stderr, "MAC must be 6 bytes!\n");
        return;
    }
    printf("%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/*
 @AF_PACKET - work on canal level (L2)
*/
void create_raw_socket() {
  int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sockfd < 0) {
    perror("socket");
    return -1;
  }
  return sockfd;
}

int send_ethernet_frame(int sockfd, EthernetFrame *frame, const char *iface) {
  struct sockaddr {
    unsigned short sa_family;
    char sa_data[14];
  }
}