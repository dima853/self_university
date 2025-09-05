#ifndef LFSR_H
#define LFSR_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    uint32_t register_state;
    uint8_t length;
    uint32_t polynomial;
} LFSR;

void lfsr_init(LFSR *lfsr, uint32_t initial_state, uint8_t length, uint32_t polynomial_mask);
uint8_t lfsr_step(LFSR *lfsr);
void lfsr_generate_sequence(LFSR *lfsr, uint8_t *output, size_t n);

#endif