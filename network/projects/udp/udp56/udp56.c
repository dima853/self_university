// 1. Connect the necessary header files
#include <sys/socket.h> // for socket(), sendto(), recvfrom()
#include <netinet/in.h> // for sockaddr_in, IPPROTO_UDP
#include <arpa/inet.h> // for inet_pton()
#include <unistd.h> // for close()
#include <string.h> // for memset()
#include <stdint.h> // for uint16_t, uint32_t
#include <stdio.h> // for printf()
#include <errno.h> // for errno

#define DNS_SERVER "8.8.8.8"  // Google Public DNS
#define DNS_PORT 53
#define BUFFER_SIZE 512
#define TIMEOUT_SEC 2

int main() {
// 2. Creating a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Fixed: it was IPPROTO_UPD
if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // 3. Setting a timeout for the socket
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("Setsockopt failed");
        close(sockfd);
        return 1;
    }

    // 4. Setting up the DNS server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DNS_PORT);
    if (inet_pton(AF_INET, DNS_SERVER, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sockfd);
        return 1;
    }

    // Here should be the code for forming a DNS query
// unsigned char dns_query[BUFFER_SIZE] = {...};
// size_t query_len = ...;

    // 5. Sending a DNS query
    // if (sendto(sockfd, dns_query, query_len, 0, 
    //           (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    //     perror("Send failed");
    //     close(sockfd);
    //     return 1;
    // }

    // 6. We receive a response
    unsigned char response[BUFFER_SIZE];
    // ssize_t resp_len = recvfrom(sockfd, response, BUFFER_SIZE, 0, NULL, NULL);
    // if (resp_len < 0) {
    //     perror("Receive failed");
    //     close(sockfd);
    //     return 1;
    // }

    // There should be a code for parsing the DNS response here

    // 7. Closing the socket
    close(sockfd);
    return 0;
}