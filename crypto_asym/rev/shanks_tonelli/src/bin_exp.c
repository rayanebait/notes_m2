#include "shanks_tonelli.h"

#include <fmpz.h>
#include <flint.h>


int main(int argc, char* argv[])
{
    if(argc != 4){
        flint_printf("Usage:\n\t -Input: <x>, <n>, <p>\n\t -Output: x**n mod p\n");
        exit(1);
    }

    fmpz_t x,n,p;
    fmpz_init(x);
    fmpz_init(n);
    fmpz_init(p);
    
    if(fmpz_set_str(x, argv[1], 10) == -1 || fmpz_set_str(n, argv[2], 10) == -1 
            || fmpz_set_str(p, argv[3], 10) == -1){
        fmpz_clear(x);
        fmpz_clear(n);
        fmpz_clear(p);
        exit(1);
    }

    fmpz_t base;
    fmpz_init_set_ui(base, 10);
    if(fmpz_is_strong_probabprime(p,base)!=1){
        fmpz_print(p);
        flint_printf(" is not prime. Enter x prime for the modulus.\n");

        fmpz_clear(x);
        fmpz_clear(n);
        fmpz_clear(p);
        fmpz_clear(base);
        exit(1);
    }
    fmpz_clear(base);

    fmpz_t y;
    fmpz_init(y);

    bin_exp_mod(y, x, n, p);

    fmpz_print(x);
    flint_printf("**");
    fmpz_print(n);
    flint_printf("=");
    fmpz_print(y);
    flint_printf(" mod ");
    fmpz_print(p);
    flint_printf("\n");


    fmpz_clear(x);
    fmpz_clear(n);
    fmpz_clear(p);
    fmpz_clear(y);
    return 0;


}