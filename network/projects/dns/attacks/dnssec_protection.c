#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define KEY_LENGTH 2048

struct dnssec_entry {
    char domain[100];
    char ip[16];
    unsigned char *signature;
    size_t sig_len;
};

EVP_PKEY *generate_keypair() {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY *pkey = NULL;
    
    if (!ctx) return NULL;
    if (EVP_PKEY_keygen_init(ctx) <= 0) goto err;
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, KEY_LENGTH) <= 0) goto err;
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) goto err;
    
err:
    EVP_PKEY_CTX_free(ctx);
    return pkey;
}

int sign_dns_entry(EVP_PKEY *priv_key, struct dnssec_entry *entry) {
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    unsigned char *sig = NULL;
    size_t sig_len;
    
    if (!md_ctx) return 0;
    
    // Вычисляем размер подписи
    if (EVP_DigestSignInit(md_ctx, NULL, EVP_sha256(), NULL, priv_key) <= 0) goto err;
    if (EVP_DigestSignUpdate(md_ctx, entry->domain, strlen(entry->domain)) <= 0) goto err;
    if (EVP_DigestSignUpdate(md_ctx, entry->ip, strlen(entry->ip)) <= 0) goto err;
    if (EVP_DigestSignFinal(md_ctx, NULL, &sig_len) <= 0) goto err;
    
    // Выделяем память для подписи
    sig = malloc(sig_len);
    if (!sig) goto err;
    
    // Создаем подпись
    if (EVP_DigestSignFinal(md_ctx, sig, &sig_len) <= 0) {
        free(sig);
        goto err;
    }
    
    entry->signature = sig;
    entry->sig_len = sig_len;
    
    EVP_MD_CTX_free(md_ctx);
    return 1;
    
err:
    EVP_MD_CTX_free(md_ctx);
    return 0;
}

int verify_dns_entry(EVP_PKEY *pub_key, struct dnssec_entry *entry) {
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    int ret = 0;
    
    if (!md_ctx) return 0;
    
    if (EVP_DigestVerifyInit(md_ctx, NULL, EVP_sha256(), NULL, pub_key) <= 0) goto out;
    if (EVP_DigestVerifyUpdate(md_ctx, entry->domain, strlen(entry->domain)) <= 0) goto out;
    if (EVP_DigestVerifyUpdate(md_ctx, entry->ip, strlen(entry->ip)) <= 0) goto out;
    ret = EVP_DigestVerifyFinal(md_ctx, entry->signature, entry->sig_len) == 1;
    
out:
    EVP_MD_CTX_free(md_ctx);
    return ret;
}

void free_dnssec_entry(struct dnssec_entry *entry) {
    free(entry->signature);
}

int main() {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    
    // Генерируем ключи
    EVP_PKEY *keypair = generate_keypair();
    if (!keypair) {
        fprintf(stderr, "Ошибка генерации ключей\n");
        ERR_print_errors_fp(stderr);
        return 1;
    }
    
    // Создаем DNS запись с DNSSEC
    struct dnssec_entry entry;
    strncpy(entry.domain, "example.com", sizeof(entry.domain) - 1);
    strncpy(entry.ip, "93.184.216.34", sizeof(entry.ip) - 1);
    entry.signature = NULL;
    
    // Подписываем запись
    if (!sign_dns_entry(keypair, &entry)) {
        fprintf(stderr, "Ошибка подписи DNS записи\n");
        ERR_print_errors_fp(stderr);
        EVP_PKEY_free(keypair);
        return 1;
    }
    printf("DNSSEC запись создана и подписана\n");
    
    // Проверяем подпись (легитимный случай)
    if (verify_dns_entry(keypair, &entry)) {
        printf("Проверка DNSSEC успешна - запись подлинная\n");
    } else {
        printf("Проверка DNSSEC не пройдена!\n");
        ERR_print_errors_fp(stderr);
    }
    
    // Имитация атаки - меняем IP
    strncpy(entry.ip, "192.168.1.100", sizeof(entry.ip) - 1);
    
    // Проверяем подпись после изменения
    if (verify_dns_entry(keypair, &entry)) {
        printf("Проверка DNSSEC успешна (этого не должно быть!)\n");
    } else {
        printf("Проверка DNSSEC не пройдена - обнаружена подделка!\n");
    }
    
    // Очистка
    free_dnssec_entry(&entry);
    EVP_PKEY_free(keypair);
    EVP_cleanup();
    ERR_free_strings();
    
    return 0;
}