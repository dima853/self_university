#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Structure to hold the state of the PRNG
typedef struct {
    uint32_t state;
    uint32_t a;
    uint32_t c;
    uint32_t m;
} prng_state;

// Initialize the PRNG state
void prng_init(prng_state *prng, uint32_t seed, uint32_t a, uint32_t c, uint32_t m) {
    prng->state = seed;
    prng->a = a;
    prng->c = c;
    prng->m = m;
}

// Generate the next pseudo-random byte
uint8_t prng_next(prng_state *prng) {
    prng->state = (prng->a * prng->state + prng->c) % prng->m;
    return (uint8_t)(prng->state & 0xFF); // Return the least significant byte
}

// Encrypt/decrypt a buffer using the stream cipher
void stream_cipher(uint8_t *data, size_t data_len, prng_state *prng) {
    for (size_t i = 0; i < data_len; i++) {
        data[i] ^= prng_next(prng); // XOR with the next keystream byte
    }
}

int main() {
    // Example usage
    uint8_t plaintext[] = "This is a secret meАssage.";
    size_t plaintext_len = sizeof(plaintext) - 1; // Exclude null terminator

    // Initialize PRNG with a seed and parameters
    prng_state prng;
    prng_init(&prng, 12345, 1664525, 1013904223, 0xFFFFFFFF);

    printf("Plaintext: %s\n", plaintext);

    // Encryption
    uint8_t ciphertext[plaintext_len];
    memcpy(ciphertext, plaintext, plaintext_len); // Copy plaintext to ciphertext buffer
    stream_cipher(ciphertext, plaintext_len, &prng);

    printf("Ciphertext: ");
    for (size_t i = 0; i < plaintext_len; i++) {
        printf("%02X", ciphertext[i]);
    }
    printf("\n");

    // Reset PRNG state for decryption (using the same seed is crucial)
    prng_init(&prng, 12345, 1664525, 1013904223, 0xFFFFFFFF);

    // Decryption
    stream_cipher(ciphertext, plaintext_len, &prng);

    printf("Decrypted text: %s\n", ciphertext);

    return 0;
}