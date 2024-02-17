#include "facto.h"

#include <gmp.h>

/*Pour calculer efficament la puissance de p 
la plus grande qui est plus petite que n. On doit 
faire une dichotomie sur l'exposantet non une exponentiation naive.*/
/*On suppose ici que le log en base p,2 de n est toujours en simple précision*/
int
compute_prime_power(mpz_t primepower, mpz_t prime, mpz_t n)
{
    if(mpz_cmp(prime, n)==0){
        mpz_set(primepower, p);
        return 1;
    } else if (mpz_cmp(prime, n)>0){
        return -1;
    }
    /*On calcule le plus grand a tq p^a<n*/
    size_t exponent_size = mpz_sizeinbase(n, mpz_get_ui(prime));
    mpz_t exponent_sizempz;
    mpz_init_set_ui(exponent_sizempz, exponent_size);

    /*on calcule ensuite le log en base 2 de a pour pouvoir initialiser 
    le minimum d'exposants*/
    exponent_size = mpz_sizeinbase(exponent_sizempz, 2);
    mpz_clear(exponent_sizempz);
    /*gmp assure que le log en base p est exact ou 1 de plus que la 
    valeur désirée ce qui assure qu'on aura pas d'overflow*/

    /*On stocke ici les puissance 2^(i)eme de prime*/
    mpz_t *exponents = malloc(sizeof(mpz_t)*(exponent_size));
    if(!exponents){
        return -1;
    }
    
    mpz_init(exponents[0]);
    mpz_mul(exponents[0], prime, prime);
    
    for(size_t i=1;  i<exponent_size; i++){
        /*exponents[i]=p^(2^i)*/
        mpz_init(exponents[i]);
        mpz_mul(exponents[i], exponents[i-1], exponents[i-1]);
    }

    mpz_set(primepower, prime);
    mpz_t tmp;
    mpz_init(tmp);
    for(size_t i=exponent_size; i>0; i--){
        /*On regarde si */
        mpz_mul(tmp, primepower, exponents[i]);
        if(mpz_cmp(tmp, n)>0){
            continue;
        }
        mpz_set(primepower, tmp);
    }

    for(size_t i=0; i<exponent_size; i++){
        mpz_clear(exponents[i]);
    }
    mpz_clear(tmp);


}
int
compute_K(mpz_t K, mpz_t n, unsigned B)
{
    /*prime2 will contain the prime after prime1*/
    mpz_t prime1, prime2, primepower;
    mpz_init_set_ui(prime1, 2);
    mpz_init(prime2);
    /*primepower is here to compute the highest power 
    below n*/
    mpz_init(primepower);
    unsigned i=0;

    mpz_set_ui(K, 1);

    while(prime1<B)
    {
        if(compute_prime_power(primepower, prime1, n)==-1){
            /*Il ya une erreur*/
            return -1;
        }
        mpz_mul(K, K, primepower);
        mpz_nextprime(prime2, prime1);
        mpz_set(prime1, prime2);
        i++;
    }

    return 0;
}

int
facto_mpz(mpz_t n, mpz_t factor, unsigned long int B)
{
    mpz_t K;
    mpz_init(K);

    if(compute(K, n, B)==-1){
        /*Il y a une erreur*/
        mpz_clear(K);
        return -1;
    };

    mpz_t a, a_K;
    mpz_init_set_ui(a, 2);
    mpz_init(a_K);

    gmp_randstate_t state;
    gmp_randinit_default(state);
    /*Ici a est choisi entre 0 et n-1 mais je ne sais pas le faire en 2 et n-2*/

    while(1){
        mpz_urandomm(a, state, n);
            /*Calcul du pgcd de a et n*/
            /*si ...*/
            /*mpz_set(factor,gcd)*/
            /*sinon calcul de a_K et calcul du pgcd*/
            /*mpz_set(factor,gcd)*/
    }

}