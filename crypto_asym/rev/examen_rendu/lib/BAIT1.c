#include "enc_scheme.h"

#include <stdlib.h>
#include <gmp.h>


mpz_t*
encrypt(mpz_t m, size_t size, mpz_t N, mpz_t z)
{
    mpz_t *c = malloc(sizeof(mpz_t)*size);
    if(!c){
        /*Allocation failed*/
        return NULL;
    }
    for(size_t i = 0; i<size;i++){
        mpz_init(c[i]);
    }

    gmp_randstate_t state;
    gmp_randinit_default(state);
    int bit=0;
    mpz_t y, tmp;
    mpz_init(y);
    mpz_init(tmp);

    for(size_t i=0; i<size; i++){
        /*generate random y between 0 and N-1*/
        mpz_urandomm(y, state, N);
        /*get m's i'th bit*/
        bit = mpz_tstbit(m, i);
        /*compute y<-y² mod N*/
        mpz_pow_ui(y, y, 2);
        mpz_mod(y, y, N);

        /*compute c_i=y²z^{m_i}*/
        if(bit){
            mpz_mul(c[i], y, z);
            mpz_mod(y, y, N);
        }
    }

    mpz_clear(tmp);
    mpz_clear(y);

    return c;
}

int decrypt_bit(mpz_t c_i, mpz_t p)
{
    /*legendre(c_i, p)=legendre(y²z^(m_i), p)=(-1)^(m_i)*/
    return mpz_legendre(c_i, p);
}

void 
decrypt(mpz_t decrypted,  mpz_t *c,size_t size, mpz_t p)
{
    mpz_t tmp;
    mpz_init(tmp);

    for(size_t i=0; i<size; i++)
    {
        if(decrypt_bit(c[i], p)){
            mpz_setbit(decrypted, i);
        }
    }

    mpz_clear(tmp);
}