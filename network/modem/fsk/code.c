#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define SAMPLE_RATE 44100
#define BIT_DURATION 0.1  // 100 мс на бит
#define FREQ_0 1070       // Частота для '0'
#define FREQ_1 1270       // Частота для '1'

// Генерирует синусоидальный сигнал для бита
void generate_tone(double freq, double duration, FILE *file) {
    int num_samples = duration * SAMPLE_RATE;
    for (int i = 0; i < num_samples; i++) {
        double t = (double)i / SAMPLE_RATE;
        double sample = sin(2 * M_PI * freq * t);
        fwrite(&sample, sizeof(double), 1, file);
    }
}

int main() {
    FILE *output = fopen("modem_signal.raw", "wb");
    if (!output) {
        perror("Ошибка открытия файла");
        return 1;
    }

    // Передаём строку "01" (для примера)
    const char *data = "01";
    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == '0') {
            generate_tone(FREQ_0, BIT_DURATION, output);
        } else if (data[i] == '1') {
            generate_tone(FREQ_1, BIT_DURATION, output);
        }
    }

    fclose(output);
    printf("Сигнал сохранён в modem_signal.raw\n");
    return 0;
}