#ifndef BM_ATTACK_H
#define BM_ATTACK_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    uint32_t coeffs;
    uint8_t degree;
} Polynomial;

void poly_init(Polynomial *poly);
void berlekamp_massey(const uint8_t *sequence, size_t n, Polynomial *result);
uint32_t recover_state(const uint8_t *sequence, size_t n, const Polynomial *poly);

#endif