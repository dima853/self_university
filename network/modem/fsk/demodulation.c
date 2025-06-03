#include <stdio.h>
#include <math.h>
#include <complex.h>

#define SAMPLE_RATE 44100
#define BIT_DURATION 0.1 
#define FREQ_0 1070
#define FREQ_1 1270

// Проверяет, какая частота доминирует в сигнале
char detect_frequency(FILE *input, double duration) {
    int num_samples = duration * SAMPLE_RATE;
    double energy_0 = 0, energy_1 = 0;

    for (int i = 0; i < num_samples; i++) {
        double sample;
        if (fread(&sample, sizeof(double), 1, input) != 1) break;

        // Анализ через преобразование Фурье (упрощённо)
        energy_0 += sample * sin(2 * M_PI * FREQ_0 * i / SAMPLE_RATE);
        energy_1 += sample * sin(2 * M_PI * FREQ_1 * i / SAMPLE_RATE);
    }

    return (energy_1 > energy_0) ? '1' : '0';
}

int main() {
    FILE *input = fopen("modem_signal.raw", "rb");
    if (!input) {
        perror("Ошибка открытия файла");
        return 1;
    }

    // Узнаём размер файла в байтах
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);

    // Вычисляем количество битов (каждый бит = SAMPLE_RATE * BIT_DURATION * sizeof(double) байт)
    int num_bits = file_size / (SAMPLE_RATE * BIT_DURATION * sizeof(double));

    printf("Декодированные биты: ");
    for (int i = 0; i < num_bits; i++) {
        char bit = detect_frequency(input, BIT_DURATION);
        printf("%c", bit);
    }

    fclose(input);
    return 0;
}