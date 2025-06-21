#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>

void handle_openssl_error() {
    ERR_print_errors_fp(stderr);
    exit(1);
}

// Генерация ключевой пары
EVP_PKEY* generate_key_pair() {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    if (!ctx) handle_openssl_error();
    
    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        handle_openssl_error();
    }
    
    // Установка параметров для P-256
    if (EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx, NID_X9_62_prime256v1) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        handle_openssl_error();
    }
    
    EVP_PKEY *key = NULL;
    if (EVP_PKEY_keygen(ctx, &key) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        handle_openssl_error();
    }
    
    EVP_PKEY_CTX_free(ctx);
    return key;
}

// Создание подписи
unsigned char* sign_message(const char *message, size_t *sig_len, EVP_PKEY *key) {
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) handle_openssl_error();
    
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)message, strlen(message), hash);
    
    if (EVP_DigestSignInit(ctx, NULL, EVP_sha256(), NULL, key) <= 0) {
        EVP_MD_CTX_free(ctx);
        handle_openssl_error();
    }
    
    // Определяем размер подписи
    if (EVP_DigestSign(ctx, NULL, sig_len, hash, SHA256_DIGEST_LENGTH) <= 0) {
        EVP_MD_CTX_free(ctx);
        handle_openssl_error();
    }
    
    // Выделяем память для подписи
    unsigned char *sig = OPENSSL_malloc(*sig_len);
    if (!sig) {
        EVP_MD_CTX_free(ctx);
        handle_openssl_error();
    }
    
    // Создаем подпись
    if (EVP_DigestSign(ctx, sig, sig_len, hash, SHA256_DIGEST_LENGTH) <= 0) {
        OPENSSL_free(sig);
        EVP_MD_CTX_free(ctx);
        handle_openssl_error();
    }
    
    EVP_MD_CTX_free(ctx);
    return sig;
}

// Проверка подписи
int verify_signature(const char *message, const unsigned char *sig, size_t sig_len, EVP_PKEY *key) {
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) handle_openssl_error();
    
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)message, strlen(message), hash);
    
    if (EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, key) <= 0) {
        EVP_MD_CTX_free(ctx);
        handle_openssl_error();
    }
    
    int result = EVP_DigestVerify(ctx, sig, sig_len, hash, SHA256_DIGEST_LENGTH);
    EVP_MD_CTX_free(ctx);
    
    if (result < 0) {
        handle_openssl_error();
    }
    
    return result;
}

// Вывод ключей в шестнадцатеричном формате
void print_keys(EVP_PKEY *key) {
    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio) handle_openssl_error();
    
    // Вывод приватного ключа
    if (!PEM_write_bio_PrivateKey(bio, key, NULL, NULL, 0, NULL, NULL)) {
        BIO_free(bio);
        handle_openssl_error();
    }
    
    char *priv_key = NULL;
    long priv_len = BIO_get_mem_data(bio, &priv_key);
    printf("Private Key:\n%.*s\n", (int)priv_len, priv_key);
    
    BIO_reset(bio);
    
    // Вывод публичного ключа
    if (!PEM_write_bio_PUBKEY(bio, key)) {
        BIO_free(bio);
        handle_openssl_error();
    }
    
    char *pub_key = NULL;
    long pub_len = BIO_get_mem_data(bio, &pub_key);
    printf("Public Key:\n%.*s\n", (int)pub_len, pub_key);
    
    BIO_free(bio);
}

int main() {
    // Инициализация OpenSSL
    if (!OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS | OPENSSL_INIT_ADD_ALL_CIPHERS | 
                            OPENSSL_INIT_ADD_ALL_DIGESTS, NULL)) {
        handle_openssl_error();
    }
    
    // Генерация ключей
    EVP_PKEY *key = generate_key_pair();
    print_keys(key);
    
    const char *message = "Hello, ECDSA!";
    printf("\nMessage: %s\n", message);
    
    // Создание подписи
    size_t sig_len;
    unsigned char *sig = sign_message(message, &sig_len, key);
    
    printf("\nSignature (length: %zu bytes):\n", sig_len);
    for (size_t i = 0; i < sig_len; i++) {
        printf("%02x", sig[i]);
    }
    printf("\n");
    
    // Проверка подписи
    int verified = verify_signature(message, sig, sig_len, key);
    printf("\nSignature verification: %s\n", verified ? "SUCCESS" : "FAILURE");
    
    // Попытка проверить измененное сообщение
    const char *wrong_message = "Hello, ECDSA?";
    int wrong_verified = verify_signature(wrong_message, sig, sig_len, key);
    printf("Wrong message verification: %s\n", wrong_verified ? "SUCCESS (BAD)" : "FAILURE (EXPECTED)");
    
    // Очистка
    OPENSSL_free(sig);
    EVP_PKEY_free(key);
    
    return 0;
}