#ifndef WEAK_PRG_H
#define WEAK_PRG_H

#include <stdint.h>
#define SEED_SIZE 42


volatile uint8_t seed[SEED_SIZE];

uint8_t get_byte(void);

void rng(void);

int uECC_rng(uint8_t*, unsigned);

#endif