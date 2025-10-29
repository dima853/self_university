#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    uint8_t *buffer;
    size_t head;
    size_t tail;
    size_t count;
    size_t capacity;
    bool overwrite; // разрешить перезапись старых данных
} ring_buffer_t;

// Создание буфера
ring_buffer_t *rb_create(size_t capacity, bool overwrite)
{
    ring_buffer_t *rb = malloc(sizeof(ring_buffer_t));
    if (!rb)
        return NULL;

    rb->buffer = malloc(capacity * sizeof(uint8_t));
    if (!rb->buffer)
    {
        free(rb);
        return NULL;
    }

    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->capacity = capacity;
    rb->overwrite = overwrite;

    return rb;
}

// Уничтожение буфера
void rb_destroy(ring_buffer_t *rb)
{
    if (rb)
    {
        free(rb->buffer);
        free(rb);
    }
}

bool rb_is_empty(const ring_buffer_t *rb)
{
    return rb->count == 0;
}

bool rb_is_full(const ring_buffer_t *rb)
{
    return rb->count == rb->capacity;
}

// Добавление с возможностью перезаписи
bool rb_push(ring_buffer_t *rb, uint8_t data)
{
    if (rb_is_full(rb))
    {
        if (!rb->overwrite)
        {
            return false; // перезапись запрещена
        }
        // перезаписываем самый старый элемент
        rb->tail = (rb->tail + 1) % rb->capacity;
        rb->count--;
    }

    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count++;
    return true;
}

bool rb_pop(ring_buffer_t *rb, uint8_t *data)
{
    if (rb_is_empty(rb))
    {
        return false;
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count--;
    return true;
}

// Добавление нескольких элементов
size_t rb_push_multiple(ring_buffer_t *rb, const uint8_t *data, size_t len)
{
    size_t pushed = 0;

    for (size_t i = 0; i < len; i++)
    {
        if (!rb_push(rb, data[i]))
        {
            break;
        }
        pushed++;
    }

    return pushed;
}

// Извлечение нескольких элементов
size_t rb_pop_multiple(ring_buffer_t *rb, uint8_t *data, size_t len)
{
    size_t popped = 0;

    for (size_t i = 0; i < len; i++)
    {
        if (!rb_pop(rb, &data[i]))
        {
            break;
        }
        popped++;
    }

    return popped;
}

void example_usage()
{
    printf("=== Простой буфер ===\n");
    ring_buffer_t rb;
    rb_init(&rb);

    // Заполняем буфер
    for (int i = 0; i < 10; i++)
    {
        if (rb_push(&rb, i))
        {
            printf("Добавлен: %d\n", i);
        }
        else
        {
            printf("Буфер полон! Не добавили: %d\n", i);
        }
    }

    // Читаем из буфера
    uint8_t data;
    while (rb_pop(&rb, &data))
    {
        printf("Извлечен: %d\n", data);
    }

    printf("\n=== Продвинутый буфер (с перезаписью) ===\n");
    ring_buffer_t *adv_rb = rb_create(5, true); // буфер на 5 элементов с перезаписью

    for (int i = 0; i < 10; i++)
    {
        rb_push(adv_rb, i);
        printf("Добавлен: %d (размер: %zu)\n", i, adv_rb->count);
    }

    printf("Содержимое буфера: ");
    while (rb_pop(adv_rb, &data))
    {
        printf("%d ", data);
    }
    printf("\n");

    rb_destroy(adv_rb);
}

int main()
{
    example_usage();
    return 0;
}