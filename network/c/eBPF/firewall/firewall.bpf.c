#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

// MAP для хранения правил блокировки IP
struct
{
    __uint(type, BPF_MAP_TYPE_LRU_HASH);
    __uint(max_entries, 1000);
    __type(key, __be32); // IP адрес для блокировки
    __type(value, __u8); // Просто флаг (1 = блокировать)
} block_list SEC(".maps");

// MAP для статистики
struct
{
    __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
    __uint(max_entries, 1);
    __type(key, __u32);
    __type(value, struct stats);
} stats_map SEC(".maps");

struct stats
{
    __u64 total_packets;
    __u64 blocked_packets;
};

SEC("xdp")
int firewall(struct xdp_md *ctx)
{
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    // Обновляем статистику
    __u32 key = 0;
    struct stats *stats = bpf_map_lookup_elem(&stats_map, &key);
    if (stats)
    {
        stats->total_packets++;
    }

    // Парсим Ethernet заголовок
    struct ethhdr *eth = data;
    if ((void *)eth + sizeof(*eth) > data_end)
        return XDP_PASS;

    // Работаем только с IP пакетами
    if (eth->h_proto != bpf_htons(ETH_P_IP))
        return XDP_PASS;

    // Парсим IP заголовок
    struct iphdr *ip = data + sizeof(*eth);
    if ((void *)ip + sizeof(*ip) > data_end)
        return XDP_PASS;

    // Проверяем IP в черном списке
    __u8 *should_block = bpf_map_lookup_elem(&block_list, &ip->saddr);
    if (should_block)
    {
        // БЛОКИРУЕМ пакет!
        if (stats)
        {
            stats->blocked_packets++;
        }
        return XDP_DROP;
    }

    // Разрешаем пакет
    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";