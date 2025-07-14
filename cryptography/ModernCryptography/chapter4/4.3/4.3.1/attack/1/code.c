#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define BLOCK_SIZE 16 // AES-128 (n = 128 бит)

// Структура для хранения запросов атакующего
typedef struct
{
    unsigned char message[BLOCK_SIZE];
    unsigned char tag[BLOCK_SIZE];
} MacQuery;

MacQuery queries[100]; // История запросов
int query_count = 0;   // Количество запросов

// Оракул O (либо F_k, либо случайная функция)
void oracle(const unsigned char *key, const unsigned char *message, unsigned char *tag, int is_random)
{
    if (is_random)
    {
        // Генерируем случайный тег (имитация случайной функции)
        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            tag[i] = rand() % 256;
        }
    }
    else
    {
        // Реальный MAC (AES-ECB)
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);
        EVP_CIPHER_CTX_set_padding(ctx, 0);
        int len;
        EVP_EncryptUpdate(ctx, tag, &len, message, BLOCK_SIZE);
        EVP_CIPHER_CTX_free(ctx);
    }
}

// Атакующий A (пытается подделать MAC)
void attacker(const unsigned char *key, int is_random)
{
    unsigned char m[BLOCK_SIZE] = "Hello, world!123";
    unsigned char t[BLOCK_SIZE];

    // 1. Атакующий запрашивает теги для некоторых сообщений
    for (int i = 0; i < 3; i++)
    {
        oracle(key, m, t, is_random);
        memcpy(queries[query_count].message, m, BLOCK_SIZE);
        memcpy(queries[query_count].tag, t, BLOCK_SIZE);
        query_count++;
    }

    // 2. Атакующий пытается подделать тег для нового сообщения
    unsigned char forged_m[BLOCK_SIZE] = "Hacked!!!1234567";
    unsigned char forged_t[BLOCK_SIZE];

    // (В реальности A мог бы использовать уязвимости, здесь для простоты берём случайный тег)
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        forged_t[i] = rand() % 256;
    }

    // 3. Distinguisher D проверяет подделку
    unsigned char t_prime[BLOCK_SIZE];
    oracle(key, forged_m, t_prime, is_random);

    int is_valid = 1;
    // Проверяем, что A не запрашивал forged_m ранее
    for (int i = 0; i < query_count; i++)
    {
        if (memcmp(forged_m, queries[i].message, BLOCK_SIZE) == 0)
        {
            is_valid = 0;
            break;
        }
    }

    // Если тег совпал и сообщение новое, D считает, что это F_k
    if (is_valid && memcmp(forged_t, t_prime, BLOCK_SIZE) == 0)
    {
        printf("D выводит 1 (предполагает, что O = F_k)\n");
    }
    else
    {
        printf("D выводит 0 (предполагает, что O случайная)\n");
    }
}

// Distinguisher D
void distinguisher(const unsigned char *key)
{
    printf("=== Тест 1: O = F_k (AES) ===\n");
    attacker(key, 0); // Оракул - реальный MAC

    printf("\n=== Тест 2: O = случайная функция ===\n");
    attacker(key, 1); // Оракул - случайная функция
}

int main()
{
    const unsigned char key[BLOCK_SIZE] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x97, 0x99, 0x89, 0xcf, 0xab, 0x12};

    distinguisher(key);
    return 0;
}