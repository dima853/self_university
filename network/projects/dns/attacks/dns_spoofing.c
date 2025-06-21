#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CACHE_SIZE 10

struct dns_entry {
    char domain[100];
    char ip[16];
    unsigned int ttl;
};

struct dns_cache {
    struct dns_entry entries[CACHE_SIZE];
    int count;
};   

void init_cache(struct dns_cache *cache) {
    cache->count = 0;
}

void add_to_cache(struct dns_cache *cache, const char *domain, const char *ip, unsigned int ttl) {
    if (cache->count >= CACHE_SIZE) return;
    
    strncpy(cache->entries[cache->count].domain, domain, sizeof(cache->entries[cache->count].domain) - 1);
    strncpy(cache->entries[cache->count].ip, ip, sizeof(cache->entries[cache->count].ip) - 1);
    cache->entries[cache->count].ttl = ttl;
    cache->count++;
}

void poison_cache(struct dns_cache *cache, const char *domain, const char *fake_ip) {
    for (int i = 0; i < cache->count; i++) {
        if (strcmp(cache->entries[i].domain, domain) == 0) {
            strncpy(cache->entries[i].ip, fake_ip, sizeof(cache->entries[i].ip) - 1);
            printf("[Атака] Кэш отравлен: %s теперь указывает на %s\n", domain, fake_ip);
            return;
        }
    }
    printf("[Атака] Домен %s не найден в кэше\n", domain);
}

void print_cache(struct dns_cache *cache) {
    printf("\nТекущее состояние DNS кэша:\n");
    printf("---------------------------------\n");
    for (int i = 0; i < cache->count; i++) {
        printf("%-20s -> %-15s (TTL: %u)\n", 
               cache->entries[i].domain, 
               cache->entries[i].ip, 
               cache->entries[i].ttl);
    }
    printf("---------------------------------\n");
}

int main() {
    struct dns_cache cache;
    init_cache(&cache);
    
    // Добавляем легитимные записи
    add_to_cache(&cache, "example.com", "93.184.216.34", 300);
    add_to_cache(&cache, "google.com", "172.217.16.206", 600);
    
    print_cache(&cache);
    
    // Имитация атаки DNS Spoofing
    poison_cache(&cache, "example.com", "192.168.1.100");
    
    print_cache(&cache);
    
    return 0;
}