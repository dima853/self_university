#include <string.h>
#include <stdbool.h>

typedef struct
{
    unsigned char bytes[6];
} mac_t;

__attribute__((always_inline)) static inline bool mac_equals_inline(const mac_t *m1, const mac_t *m2)
{
    return memcmp(m1->bytes, m2->bytes, 6) == 0;
}

__attribute__((noinline)) static bool mac_equals_normal(const mac_t *m1, const mac_t *m2)
{
    return memcmp(m1->bytes, m2->bytes, 6) == 0;
}

int test_with_inline(mac_t *mac1, mac_t *mac2, mac_t *mac3)
{
    int result = 0;
    if (mac_equals_inline(mac1, mac2))
        result += 1;
    if (mac_equals_inline(mac2, mac3))
        result += 2;
    if (mac_equals_inline(mac1, mac3))
        result += 4;
    if (mac_equals_inline(mac2, mac1))
        result += 8;
    return result;
}

int test_without_inline(mac_t *mac1, mac_t *mac2, mac_t *mac3)
{
    int result = 0;
    if (mac_equals_normal(mac1, mac2))
        result += 1;
    if (mac_equals_normal(mac2, mac3))
        result += 2;
    if (mac_equals_normal(mac1, mac3))
        result += 4;
    if (mac_equals_normal(mac2, mac1))
        result += 8;
    return result;
}