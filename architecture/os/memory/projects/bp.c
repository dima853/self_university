#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define SIGNATURE 0x4D594654

/*
  bp - Brain Dump
*/
void bp(const char* filename, const char* text) {
    FILE* file = fopen(filename, "wb"); 
    
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    uint32_t signature = SIGNATURE;
    fwrite(&signature, sizeof(uint32_t), 1, file);

    uint32_t text_length = strlen(text);
    fwrite(&text_length, sizeof(uint32_t), 1, file);

    fwrite(text, sizeof(char), text_length, file);

    fclose(file);
}

int main() {
    const char* text = "Привет, это мой формат файла!";
    bp("example.dp", text);
    return 0;
}