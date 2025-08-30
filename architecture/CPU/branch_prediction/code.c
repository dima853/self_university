#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

// Количество "пакетов" для обработки
#define NUM_PACKETS 100000000

// Типы пакетов (их "айди")
typedef enum
{
    PACKET_TYPE_DATA = 0,  // 70% трафика
    PACKET_TYPE_ACK,       // 20% трафика
    PACKET_TYPE_MGMT,      // 5% трафика
    PACKET_TYPE_KEEPALIVE, // 4% трафика
    PACKET_TYPE_ERROR,     // 1% трафика
    PACKET_TYPE_LAST
} packet_type_t;

// Структура "пакета"
typedef struct
{
    packet_type_t type;
    uint32_t payload;
} packet_t;

// Заполняем массив пакетов согласно заданному распределению
void generate_packets(packet_t *packets, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        int r = rand() % 100; // Случайное число от 0 до 99

        if (r < 70)
        {
            packets[i].type = PACKET_TYPE_DATA; // 70%
            packets[i].payload = rand();
        }
        else if (r < 90)
        {
            packets[i].type = PACKET_TYPE_ACK; // 20%
            packets[i].payload = rand();
        }
        else if (r < 95)
        {
            packets[i].type = PACKET_TYPE_MGMT; // 5%
            packets[i].payload = rand();
        }
        else if (r < 99)
        {
            packets[i].type = PACKET_TYPE_KEEPALIVE; // 4%
            packets[i].payload = rand();
        }
        else
        {
            packets[i].type = PACKET_TYPE_ERROR; // 1%
            packets[i].payload = rand();
        }
    }
}

// Обработчики для каждого типа пакета (делают какую-то "работу")
void process_data_packet(uint32_t payload)
{
    volatile uint32_t result = payload * 2; // volatile чтобы компилятор не оптимизировал
    (void)result;                           // Используем результат чтобы избежать оптимизации
}

void process_ack_packet(uint32_t payload)
{
    volatile uint32_t result = payload + 1;
    (void)result;
}

void process_mgmt_packet(uint32_t payload)
{
    volatile uint32_t result = payload / 2;
    (void)result;
}

void process_keepalive_packet(uint32_t payload)
{
    volatile uint32_t result = payload & 0xFF;
    (void)result;
}

void process_error_packet(uint32_t payload)
{
    volatile uint32_t result = ~payload;
    (void)result;
}

// 1. НАИВНАЯ ОБРАБОТКА (плохой порядок)
uint64_t process_packets_naive(packet_t *packets, size_t n)
{
    uint64_t total_work = 0;
    for (size_t i = 0; i < n; i++)
    {
        // ❌ ПЛОХО: Самый частый тип (DATA) проверяется последним!
        if (packets[i].type == PACKET_TYPE_ERROR)
        { // 1% случаев
            process_error_packet(packets[i].payload);
            total_work += 100; // Предположим, обработка ошибки тяжелая
        }
        else if (packets[i].type == PACKET_TYPE_KEEPALIVE)
        { // 4%
            process_keepalive_packet(packets[i].payload);
            total_work += 5;
        }
        else if (packets[i].type == PACKET_TYPE_MGMT)
        { // 5%
            process_mgmt_packet(packets[i].payload);
            total_work += 20;
        }
        else if (packets[i].type == PACKET_TYPE_ACK)
        { // 20%
            process_ack_packet(packets[i].payload);
            total_work += 10;
        }
        else if (packets[i].type == PACKET_TYPE_DATA)
        { // 70% (должен быть первым!)
            process_data_packet(packets[i].payload);
            total_work += 15;
        }
    }
    return total_work;
}

// 2. ОПТИМИЗИРОВАННАЯ ОБРАБОТКА (хороший порядок)
uint64_t process_packets_optimized(packet_t *packets, size_t n)
{
    uint64_t total_work = 0;
    for (size_t i = 0; i < n; i++)
    {
        // ✅ ХОРОШО: Проверяем по убыванию вероятности
        if (packets[i].type == PACKET_TYPE_DATA)
        { // 70% случаев
            process_data_packet(packets[i].payload);
            total_work += 15;
        }
        else if (packets[i].type == PACKET_TYPE_ACK)
        { // 20%
            process_ack_packet(packets[i].payload);
            total_work += 10;
        }
        else if (packets[i].type == PACKET_TYPE_MGMT)
        { // 5%
            process_mgmt_packet(packets[i].payload);
            total_work += 20;
        }
        else if (packets[i].type == PACKET_TYPE_KEEPALIVE)
        { // 4%
            process_keepalive_packet(packets[i].payload);
            total_work += 5;
        }
        else if (packets[i].type == PACKET_TYPE_ERROR)
        { // 1%
            process_error_packet(packets[i].payload);
            total_work += 100;
        }
    }
    return total_work;
}

// 3. ОБРАБОТКА ЧЕРЕЗ SWITCH (для сравнения)
uint64_t process_packets_switch(packet_t *packets, size_t n)
{
    uint64_t total_work = 0;
    for (size_t i = 0; i < n; i++)
    {
        switch (packets[i].type)
        {
        case PACKET_TYPE_DATA:
            process_data_packet(packets[i].payload);
            total_work += 15;
            break;
        case PACKET_TYPE_ACK:
            process_ack_packet(packets[i].payload);
            total_work += 10;
            break;
        case PACKET_TYPE_MGMT:
            process_mgmt_packet(packets[i].payload);
            total_work += 20;
            break;
        case PACKET_TYPE_KEEPALIVE:
            process_keepalive_packet(packets[i].payload);
            total_work += 5;
            break;
        case PACKET_TYPE_ERROR:
            process_error_packet(packets[i].payload);
            total_work += 100;
            break;
        default:
            break;
        }
    }
    return total_work;
}

int main()
{
    packet_t *packets = (packet_t *)malloc(NUM_PACKETS * sizeof(packet_t));
    if (packets == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    srand(time(NULL));
    generate_packets(packets, NUM_PACKETS);

    clock_t start, end;
    double cpu_time_used;
    uint64_t result;

    printf("Processing %d packets...\n", NUM_PACKETS);

    // Тест 1: Наивная обработка (плохой порядок)
    start = clock();
    result = process_packets_naive(packets, NUM_PACKETS);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Naive (bad order): %.2f sec, Total work: %lu\n", cpu_time_used, result);

    // Тест 2: Оптимизированная обработка (хороший порядок)
    start = clock();
    result = process_packets_optimized(packets, NUM_PACKETS);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Optimized (good order): %.2f sec, Total work: %lu\n", cpu_time_used, result);

    // Тест 3: Обработка через switch
    start = clock();
    result = process_packets_switch(packets, NUM_PACKETS);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Switch statement: %.2f sec, Total work: %lu\n", cpu_time_used, result);

    free(packets);
    return 0;
}