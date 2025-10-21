#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <linux/if_link.h>

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

int main(int argc, char **argv)
{
    struct bpf_object *obj;
    int prog_fd, block_map_fd, stats_map_fd;
    int ifindex;
    char ifname[IFNAMSIZ] = "eth0"; // Интерфейс по умолчанию

    if (argc > 1)
    {
        strncpy(ifname, argv[1], IFNAMSIZ - 1);
    }

    // Загружаем eBPF программу
    obj = bpf_object__open("firewall.bpf.o");
    if (!obj)
    {
        fprintf(stderr, "❌ Ошибка загрузки eBPF объекта\n");
        return 1;
    }

    if (bpf_object__load(obj))
    {
        fprintf(stderr, "❌ Ошибка загрузки программы в ядро\n");
        return 1;
    }

    // Получаем файловые дескрипторы
    prog_fd = bpf_program__fd(bpf_object__find_program_by_name(obj, "firewall"));
    block_map_fd = bpf_object__find_map_fd_by_name(obj, "block_list");
    stats_map_fd = bpf_object__find_map_fd_by_name(obj, "stats_map");

    // Получаем индекс интерфейса
    ifindex = if_nametoindex(ifname);
    if (!ifindex)
    {
        fprintf(stderr, "❌ Интерфейс %s не найден\n", ifname);
        return 1;
    }

    // Прикрепляем XDP программу к интерфейсу
    if (bpf_set_link_xdp_fd(ifindex, prog_fd, 0) < 0)
    {
        fprintf(stderr, "❌ Ошибка прикрепления XDP к %s\n", ifname);
        return 1;
    }

    printf("✅ Фаервол запущен на интерфейсе %s\n", ifname);
    printf("🛡️ Используй команды:\n");
    printf("   block <IP>    - заблокировать IP\n");
    printf("   unblock <IP>  - разблокировать IP\n");
    printf("   stats         - показать статистику\n");
    printf("   exit          - выйти\n\n");

    // Командный интерфейс
    char command[64];
    char ip_str[INET_ADDRSTRLEN];
    __be32 ip_addr;

    while (1)
    {
        printf("firewall> ");
        if (!fgets(command, sizeof(command), stdin))
            break;

        // Убираем символ новой строки
        command[strcspn(command, "\n")] = 0;

        if (strncmp(command, "block ", 6) == 0)
        {
            // Блокируем IP
            strcpy(ip_str, command + 6);
            if (inet_pton(AF_INET, ip_str, &ip_addr) != 1)
            {
                printf("❌ Неверный IP адрес: %s\n", ip_str);
                continue;
            }

            __u8 value = 1;
            if (bpf_map_update_elem(block_map_fd, &ip_addr, &value, BPF_ANY) == 0)
            {
                printf("✅ Заблокирован IP: %s\n", ip_str);
            }
            else
            {
                printf("❌ Ошибка блокировки IP\n");
            }
        }
        else if (strncmp(command, "unblock ", 8) == 0)
        {
            // Разблокируем IP
            strcpy(ip_str, command + 8);
            if (inet_pton(AF_INET, ip_str, &ip_addr) != 1)
            {
                printf("❌ Неверный IP адрес: %s\n", ip_str);
                continue;
            }

            if (bpf_map_delete_elem(block_map_fd, &ip_addr) == 0)
            {
                printf("✅ Разблокирован IP: %s\n", ip_str);
            }
            else
            {
                printf("❌ IP не найден в черном списке: %s\n", ip_str);
            }
        }
        else if (strcmp(command, "stats") == 0)
        {
            // Показываем статистику
            __u32 key = 0;
            struct stats stats = {0};

            if (bpf_map_lookup_elem(stats_map_fd, &key, &stats) == 0)
            {
                printf("📊 Статистика:\n");
                printf("   Всего пакетов: %llu\n", stats.total_packets);
                printf("   Заблокировано: %llu\n", stats.blocked_packets);
                if (stats.total_packets > 0)
                {
                    printf("   Процент блокировки: %.2f%%\n",
                           (double)stats.blocked_packets * 100 / stats.total_packets);
                }
            }
        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else if (strcmp(command, "") != 0)
        {
            printf("❌ Неизвестная команда: %s\n", command);
        }
    }

    // Открепляем программу при выходе
    bpf_set_link_xdp_fd(ifindex, -1, 0);
    printf("\n🛑 Фаервол остановлен\n");

    return 0;
}