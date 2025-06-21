#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *names[] = {"Miller","Jones","Anderson"};
    printf("%c\n", *(*names + 1) + 3);
    printf("%c\n", *(*names + 1) + 3);
    printf("%c\n", names[1][3]);

    int num = 3;
    int *pi = &num;

    printf("Address of num: %d Value: %d\n",&num, num);
    printf("Address of pi: %d Value: %d\n",&pi, pi);
    printf("Value of pi: %d\n", (void*)pi);

    int *ptr = 0;  
    if (ptr == NULL) {
        printf("Указатель не ссылается на данные!\n");
    }
    
    char empty_str[] = "";  // Содержит только '\0'
    printf("Длина: %zu\n", strlen(empty_str));  // Выведет 0

    char str[] = {'H', 'e', 'l', 'l', 'o', '\0'};  // Строка в C
    // Эквивалентная запись:
    char str2[] = "Hello";  // Компилятор сам добавит '\0' в конец

    char broken_str[] = {'H', 'e', 'l', 'l', 'o'};  // Нет '\0'!
    printf("%s\n", broken_str);  // Может вывести "Hello☠☠☠%^&*" (мусор) 

    const char *s = "Hello";
    while (*s != '\0') {  // Пока не конец строки
        putchar(*s);
        s++;
    }

    return 0;
}