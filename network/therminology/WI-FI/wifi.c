#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <dirent.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t frame_control;
    uint16_t duration;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq_ctrl;
} wifi_mac_header_t;

typedef struct {
    wifi_mac_header_t header;
    uint8_t payload[0];
} wifi_frame_t;
#pragma pack(pop)

// Функция для поиска Wi-Fi интерфейса
char* find_wifi_interface() {
    DIR *dir;
    struct dirent *ent;
    char *interface = NULL;
    
    if ((dir = opendir("/sys/class/net/")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, "wl") != NULL || 
                strstr(ent->d_name, "wlan") != NULL) {
                interface = strdup(ent->d_name);
                break;
            }
        }
        closedir(dir);
    }
    return interface;
}

int create_raw_socket() {
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return sock;
}

void send_beacon_frame(int sock, const char* iface, const uint8_t* bssid, const char* ssid) {
    struct sockaddr_ll sa;
    struct ifreq ifr;

    memset(&sa, 0, sizeof(sa));
    memset(&ifr, 0, sizeof(ifr));

    strncpy(ifr.ifr_name, iface, IFNAMSIZ);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) == -1) {
        perror("ioctl(SIOCGIFINDEX) failed");
        return;
    }

    sa.sll_family = AF_PACKET;
    sa.sll_ifindex = ifr.ifr_ifindex;
    sa.sll_halen = ETH_ALEN;
    memcpy(sa.sll_addr, "\xff\xff\xff\xff\xff\xff", ETH_ALEN);

    uint8_t beacon_packet[] = {
        0x80, 0x00,                         // Frame Control (Beacon)
        0x00, 0x00,                         // Duration
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination MAC
        0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, // Source MAC
        0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, // BSSID
        0x00, 0x00,                         // Sequence Control
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // Timestamp
        0x64, 0x00,                                     // Beacon Interval
        0x31, 0x04,                                     // Capability Info
        0x00,                                           // SSID Tag
        (uint8_t)strlen(ssid)                          // SSID Length
    };

    uint8_t full_packet[sizeof(beacon_packet) + strlen(ssid)];
    memcpy(full_packet, beacon_packet, sizeof(beacon_packet));
    memcpy(full_packet + sizeof(beacon_packet), ssid, strlen(ssid));

    if (sendto(sock, full_packet, sizeof(full_packet), 0, 
        (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        perror("sendto failed");
    }
}

int main() {
    char *iface = find_wifi_interface();
    if (!iface) {
        fprintf(stderr, "Could not find any Wi-Fi interface\n");
        fprintf(stderr, "Available interfaces:\n");
        system("ls /sys/class/net/");
        return 1;
    }

    printf("Using interface: %s\n", iface);
    
    int sock = create_raw_socket();
    uint8_t bssid[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
    const char* ssid = "MY_WIFI";

    while (1) {
        send_beacon_frame(sock, iface, bssid, ssid);
        printf("Beacon sent via %s: %s\n", iface, ssid);
        sleep(1);
    }

    close(sock);
    free(iface);
    return 0;
}