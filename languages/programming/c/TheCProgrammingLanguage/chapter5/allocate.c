#include <stdio.h>
#include <string.h>

#define ALLOCSIZE 10000 // Общий размер доступной памяти

static char allocbuf[ALLOCSIZE]; // Статический буфер для аллокатора
static char *allocp = allocbuf;  // Указатель на следующую свободную позицию

char *alloc(int n)
{
    if (allocbuf + ALLOCSIZE - allocp >= n)
    {                      // Проверяем, достаточно ли места
        allocp += n;       // Передвигаем указатель
        return allocp - n; // Возвращаем начало выделенного блока
    }
    return NULL; // Недостаточно памяти
}

void afree(char *p)
{
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
    {               // Проверяем, что p внутри нашего буфера
        allocp = p; // "Освобождаем" память, сдвигая указатель
    }
}

int main()
{

    printf("Весь буфер: %p - %p\n", (void *)allocbuf, (void *)(allocbuf + ALLOCSIZE));
    printf("Текущая позиция: %p\n", (void *)allocp);

    char *str1 = alloc(20);
    if (str1 != NULL)
    {
        strcpy(str1, "Hello, World!");
        printf("Выделено 20 байт по адресу %p: %s\n", (void *)str1, str1);
    }

    printf("Текущая позиция: %p\n", (void *)allocp);

    char *str2 = alloc(30);
    if (str2 != NULL)
    {
        strcpy(str2, "This is a test string.");
        printf("Выделено 30 байт по адресу %p: %s\n", (void *)str2, str2);
    }

    printf("Текущая позиция: %p\n", (void *)allocp);

    // Освобождаем память (на самом деле просто сдвигаем указатель)
    printf("Освобождаем память...\n");
    afree(str1);
    printf("Текущая позиция: %p\n", (void *)allocp);

    // Пытаемся выделить слишком большой блок
    char *big_block = alloc(ALLOCSIZE + 1);
    if (big_block == NULL)
    {
        printf("Не удалось выделить %d байт (слишком большой запрос)\n", ALLOCSIZE + 1);
    }

    return 0;
}