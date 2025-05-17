#include <stdio.h>
#include <string.h>

void caesar_decrypt(char *text, int shift) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = ((text[i] - 'A' - shift + 26) % 26) + 'A';
        } else if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = ((text[i] - 'a' - shift + 26) % 26) + 'a';
        }
    }
}

int main() {
    char ciphertext[] = "Khoor, Zruog!";
    int shift = 3; // Подбираем сдвиг
    
    caesar_decrypt(ciphertext, shift);
    printf("Decrypted: %s\n", ciphertext); // Hello, World!
    
    return 0;
}