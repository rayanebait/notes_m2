#include "naive_eucl.h"
#include <fmpz.h>


void
naive_eucl(fmpz_t a, fmpz_t b, fmpz_t d)
{
    /*Assumes non NULL pointers*/
    if(fmpz_is_zero(b)){
        fmpz_zero(d);
    }

    /*uses d as r*/
    int steps = 0;
    while(!fmpz_is_zero(b)){
        steps++;
        fmpz_mod(d, a, b);
        fmpz_set(a, b);
        fmpz_set(b, d);
    }
    flint_printf("nombre d'étapes: %d", steps);

    fmpz_set(d,a);
}