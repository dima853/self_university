#include <stdio.h>
#include <string.h>

void xor_decrypt(const char *hex, unsigned char key) {
    size_t len = strlen(hex) / 2;
    unsigned char bytes[len];
    
    // Hex -> Bytes
    for (size_t i = 0; i < len; i++) {
        sscanf(hex + 2*i, "%2hhx", &bytes[i]);
    }
    
    // XOR each byte
    printf("Key %u: ", key);
    for (size_t i = 0; i < len; i++) {
        printf("%c", bytes[i] ^ key);
    }
    printf("\n");
}

int main() {
    const char *ciphertext = "1c0111001f010100061a024b53535009181c";
    
    // Bruteforce all 1-byte keys (0-255)
    for (unsigned key = 0; key < 256; key++) {
        xor_decrypt(ciphertext, key);
    }
    
    return 0;
}