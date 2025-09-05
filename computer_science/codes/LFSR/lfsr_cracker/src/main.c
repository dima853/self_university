int main()
{
    printf("=== LFSR Crack Tool ===\n\n");

    // Используем примитивный полином!
    LFSR lfsr;
    lfsr_init(&lfsr, 0b1011, 4, 0b10011); // Полином x⁴ + x + 1 (0x13)

    // Генерация последовательности
    uint8_t sequence[20];
    lfsr_generate_sequence(&lfsr, sequence, 20);

    printf("Generated sequence: ");
    for (int i = 0; i < 20; i++)
    {
        printf("%d", sequence[i]);
    }
    printf("\n\n");

    // Атака
    Polynomial attacked_poly;
    berlekamp_massey(sequence, 20, &attacked_poly);

    printf("Attacked polynomial: 0x%X\n", attacked_poly.coeffs);
    printf("Degree: %d\n", attacked_poly.degree);

    uint32_t recovered_state = recover_state(sequence, 20, &attacked_poly);
    printf("Recovered state: 0x%X\n", recovered_state);
    printf("Original state: 0x%X\n", 0b1011);

    printf("Attack %s!\n", (recovered_state == 0b1011) ? "SUCCESS" : "FAILED");

    return 0;
}