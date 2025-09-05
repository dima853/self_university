#include <stdio.h>

int calculateErrorPosition(int p1, int p2, int p3, int p4, int d1, int d2, int d3, int d4)
{
    // Проверяем контрольные суммы
    int s1 = (p1 + d1 + d2 + d4) % 2;
    int s2 = (p2 + d1 + d3 + d4) % 2;
    int s3 = (p4 + d2 + d3 + d4) % 2;

    // Собираем синдром (s3 s2 s1) и переводим в десятичный вид
    int error_position = s3 * 4 + s2 * 2 + s1;

    return error_position;
}

int main()
{
    // Пример: приняли слово с ошибкой в 3-м бите (d1)
    // p1=1, p2=0, p4=0, d1=0 (было 1), d2=1, d3=0, d4=1
    int error_pos = calculateErrorPosition(1, 0, 0, 0, 0, 1, 0, 1);

    printf("Ошибка в бите номер: %d\n", error_pos);
    // Выведет: "Ошибка в бите номер: 3"

    return 0;
}