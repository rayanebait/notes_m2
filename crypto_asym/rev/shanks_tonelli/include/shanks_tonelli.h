#ifndef SHANKS_TONELLI_H
#define SHANKS_TONELLI_H

#include <fmpz.h>

void
bin_exp_mod(fmpz_t y, fmpz_t x, fmpz_t n, fmpz_t p);

void 
shanks_tonelli(fmpz_t x, fmpz_t a, fmpz_t p);

#endif