#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define BLOCK_SIZE 16 // AES-128 (n = 128 бит = 16 байт)

void mac(const unsigned char *key, const unsigned char *message, unsigned char *tag)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        fprintf(stderr, "error create evp");
        return;
    }

    /*
    int EVP_EncryptInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type,
                        ENGINE *impl, const unsigned char *key, const unsigned char *iv);
    */
    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL) != 1)
    {
        fprintf(stderr, "error init cipher");
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    /*
    Padding is the addition of a message to the block size (if it is shorter).

    In Theorem 4.6, the message is strictly fixed in length (BLOCK_SIZE), so padding is not needed (disable).

    If there was a message shorter than 16 bytes, AES would have broken without padding.
    */
    EVP_CIPHER_CTX_set_padding(ctx, 0); // Disabling padding

    /*
    ❗In fact, this is the calculation of the tag as t = AS_k(m) (just like in Construction 4.5).

    int EVP_EncryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out,
                       int *outl, const unsigned char *in, int inl);

    ctx: context with settings (AES-128-ECB + key).

    tag: the output buffer where the result will be written (16 bytes).

    &len: The number of bytes written will be written here (it should be 16).

    message: The input message (exactly 16 bytes).
    */
    int len;
    if (EVP_EncryptUpdate(ctx, tag, &len, message, BLOCK_SIZE) != 1)
    {
        fprintf(stderr, "error cipher\n");
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    EVP_CIPHER_CTX_free(ctx); // free context
}

/*
Respond Vrfy(k,m,t)=1 only if t=Fk(m). (construction 4.5)

The memcmp function is a standard C library expression for comparing two memories.
The function is used to compare two memory blocks of the same size (in bytes)
using the so-called “lexicographic sorting". memcmp is being declared
*/
int verify(const unsigned char *key, const unsigned char *message, const unsigned char *tag)
{
    unsigned char computed_tag[BLOCK_SIZE];
    mac(key, message, computed_tag);
    return memcmp(tag, computed_tag, BLOCK_SIZE) == 0;
}

/*
🧱 Why does the architecture comply with Theorem 4.6?

    Construction 4.5 says:

        Let FF be a pseudorandom function (PRF).

        Then the MAC for the mm message is t=Fk(m)t=Fk(m).

        Check: t=?Fk(m)t=?Fk(m).

    In our code:

        FF is AES-128 in ECB mode.

        kk is a 128-bit key.

        The mm message is exactly 16 bytes.

        tt is the AES output (16 bytes).

    Why is AES a PRF?

        The theorem assumes that FF is a pseudorandom function.

        AES is considered a cryptographically strong PRF for a fixed input size (128 bits).

        ECB mode is safe for only one block (it is unsafe for multiple blocks!).

    Fixed length

        Theorem 4.6 only works for fixed-length messages (here 16 bytes).

        Other constructions are needed for messages of arbitrary length (HMAC, CBC-MAC).

❗ Important warnings

    ECB is not safe for long messages!

        If the message is larger than 16 bytes, ECB decomposes it into blocks and encrypts each one independently.

        This leads to information leakage (identical blocks produce identical ciphertexts).

        Solution: Use CBC-MAC, HMAC or AES-GCM.

    The key must be secret.

        If an attacker finds out the key, they can forge tags.

    The tag must be checked in constant time.

        memcmp compares bytes before the first mismatch, which is vulnerable to timing attacks.

        In real code, use CRYPTO_memcmp (from OpenSSL) or another secure check.
*/
int main()
{
    /*
    The key is 16 bytes (a specific example from the AES standard).

    In real code, the key must be secret (generated randomly and stored in secure memory).
    */
    const unsigned char key[BLOCK_SIZE] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x97, 0x99, 0x89, 0xcf, 0xab, 0x12};

    const unsigned char message[BLOCK_SIZE] = "Hello, world!123";

    unsigned char tag[BLOCK_SIZE];
    mac(key, message, tag);

    int is_valid = verify(key, message, tag);
    printf("Verification: %s\n", is_valid ? "SUCCESS" : "FAILED");

    unsigned char forged_message[BLOCK_SIZE] = "Hacked!!!1234567";
    int is_forged_valid = verify(key, forged_message, tag);
    printf("Forgery attempt: %s\n", is_forged_valid ? "SUCCESS (опасно!)" : "FAILED (ожидаемо)");
}