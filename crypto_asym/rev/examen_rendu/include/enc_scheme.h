#ifndef ENC_SCHEME_H
#define ENC_SCHEME_H
#include <gmp.h>

void
encrypt(mpz_t m, mpz_t N, mpz_t z);

void 
decrypt(mpz_t c, mpz_t p);

#endif