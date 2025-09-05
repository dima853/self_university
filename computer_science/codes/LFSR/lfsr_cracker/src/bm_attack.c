#include "bm_attack.h"
#include <string.h>

void poly_init(Polynomial *poly)
{
    poly->coeffs = 1;
    poly->degree = 0;
}

void berlekamp_massey(const uint8_t *sequence, size_t n, Polynomial *result)
{
    Polynomial C, B;
    poly_init(&C);
    poly_init(&B);

    uint8_t L = 0;
    uint32_t m = 1;
    // uint8_t delta = 1; // Удаляем неиспользуемую переменную

    for (size_t i = 0; i < n; i++)
    {
        uint8_t d = sequence[i];
        for (uint8_t j = 1; j <= L; j++)
        {
            if (j > i)
                break;
            if ((C.coeffs >> j) & 1)
            {
                d ^= sequence[i - j];
            }
        }

        if (d == 0)
        {
            m++;
        }
        else
        {
            Polynomial T = C;
            uint32_t shift_mask = (B.coeffs << m);
            C.coeffs ^= shift_mask;

            uint8_t new_degree = 0;
            uint32_t temp = C.coeffs;
            while (temp)
            {
                new_degree++;
                temp >>= 1;
            }
            C.degree = new_degree - 1;

            if (2 * L <= i)
            {
                B = T;
                // delta = d; // Эта строка больше не нужна
                L = i + 1 - L;
                m = 1;
            }
            else
            {
                m++;
            }
        }
    }

    *result = C;
}

uint32_t recover_state(const uint8_t *sequence, size_t n, const Polynomial *poly)
{
    uint32_t state = 0;
    uint8_t L = poly->degree;

    for (uint8_t i = 0; i < L; i++)
    {
        if (n - 1 - i < n)
        {
            state |= (sequence[n - 1 - i] << i);
        }
    }

    return state;
}