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

    // Читаем сигнатуру
    uint32_t signature;
    fread(&signature, sizeof(uint32_t), 1, file);

    if (signature != SIGNATURE) {
        fprintf(stderr, "Это не .db файл!\n");
        fclose(file);
        return NULL;
    }

    // Читаем длину текст
    uint32_t text_length;
    fread(&text_length, sizeof(uint32_t), 1, file);

    // Выделяем память под текст (+1 для '\0')
    char* text = malloc(text_length + 1);
    if (!text) {
        perror("Ошибка выделения памяти");
        fclose(file);
        return NULL;
    }

    // Читаем текст
    fread(text, sizeof(char), text_length, file);
    text[text_length] = '\0'; // Добавляем нуль-терминатор

    fclose(file);
    return text;
}

int main() {
    char* text = read_my_format("example.dp");
    
    if (text) {
        printf("Прочитанный текст: %s\n", text);
        free(text); // Не забываем освободить память!
    }

    return 0;
}