#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SIGNATURE 0x4D594654

char* read_my_format(const char* filename) {
    FILE* file = fopen(filename, "rb");
    
    if (!file) {
        perror("Ошибка открытия файла");
        return NULL;
    }

    uint32_t signature;
    fread(&signature, sizeof(uint32_t), 1, file);

    if (signature != SIGNATURE) {
        fprintf(stderr, "Это не .db файл!\n");
        fclose(file);
        return NULL;
    }

    uint32_t text_length;
    fread(&text_length, sizeof(uint32_t), 1, file);

    char* text = malloc(text_length + 1);
    if (!text) {
        perror("Ошибка выделения памяти");
        fclose(file);
        return NULL;
    }

    fread(text, sizeof(char), text_length, file);
    text[text_length] = '\0';

    fclose(file);
    return text;
}

int main() {
    char* text = read_my_format("example.dp");
    
    if (text) {
        printf("Прочитанный текст: %s\n", text);
        free(text); 
    }

    return 0;
}