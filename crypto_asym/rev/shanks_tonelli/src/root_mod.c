#include "error.h"
#include "shanks_tonelli.h"

#include <fmpz.h>
#include <flint.h>


/*Fonctionne pas mais tant pis*/

int main(int argc, char *argv[]){
    if(argc != 3){
        flint_printf("Usage:\n\t -Input: <a>, <p>\n\t -Output: x tel que\n\n\t\t x**2=a mod p\n\n");
        exit(1);
    }

    fmpz_t a,p;
    fmpz_init(a);
    fmpz_init(p);
    
    if(fmpz_set_str(a, argv[1], 10) == -1 || fmpz_set_str(p, argv[2], 10) == -1){
        fmpz_clear(a);
        fmpz_clear(p);
        exit(1);
    }

    fmpz_t base;
    fmpz_init_set_ui(base, 10);
    if(fmpz_is_strong_probabprime(p,base)!=1){
        fmpz_print(p);
        flint_printf(" is not prime. Enter x prime for the modulus.\n");

        fmpz_clear(a);
        fmpz_clear(p);
        fmpz_clear(base);
        exit(1);
    }
    fmpz_clear(base);

    fmpz_t x;
    fmpz_init(x);

    shanks_tonelli(x, a, p);

    fmpz_print(x);
    fmpz_mul(x, x, x);
    fmpz_mod(x, x, p);
    flint_printf("**2=");
    fmpz_print(x);
    flint_printf(" mod ");
    fmpz_print(p);
    flint_printf("\n");



    fmpz_clear(a);
    fmpz_clear(x);
    fmpz_clear(p);
    return 0;


}