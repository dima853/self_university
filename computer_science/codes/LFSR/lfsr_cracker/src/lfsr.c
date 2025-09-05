#include "lfsr.h"
#include <stddef.h>

void lfsr_init(LFSR *lfsr, uint32_t initial_state, uint8_t length, uint32_t polynomial_mask)
{
    lfsr->register_state = initial_state;
    lfsr->length = length;
    lfsr->polynomial = polynomial_mask;
}

uint8_t lfsr_step(LFSR *lfsr)
{
    uint8_t output_bit = lfsr->register_state & 1;

    // Правильный расчет feedback
    uint8_t feedback = 0;
    for (int i = 0; i < lfsr->length; i++)
    {
        if ((lfsr->polynomial >> i) & 1)
        {
            feedback ^= (lfsr->register_state >> i) & 1;
        }
    }

    lfsr->register_state = (lfsr->register_state >> 1) | (feedback << (lfsr->length - 1));
    return output_bit;
}

void lfsr_generate_sequence(LFSR *lfsr, uint8_t *output, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        output[i] = lfsr_step(lfsr);
    }
}