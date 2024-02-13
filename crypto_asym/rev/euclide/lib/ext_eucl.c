#include "ext_eucl.h"

#include <fmpz.h>

/*Assumes a,b,u,d are initialized*/
void
ext_eucl(fmpz_t a, fmpz_t b, fmpz_t u, fmpz_t v, fmpz_t d)
{
    fmpz_t t1, v1, t3, v3, q;

    fmpz_init(t1);
    fmpz_init_set_ui(v1, 0);
    fmpz_init(t3);
    fmpz_init_set(v3,b);
    fmpz_init(q);

    fmpz_set_ui(u, 1);
    fmpz_set(d, a);

    int nb_steps = 0;
    while(!fmpz_is_zero(v3))
    {
        nb_steps++;

        /*d=v3*q+t3*/
        fmpz_fdiv_qr(q, t3, d,v3);
        fmpz_set(d, v3);
        fmpz_set(v3, t3);

        /*t1<-u-qv1*/
        fmpz_set(t1, u);
        fmpz_submul(t1, q, v1);
        fmpz_set(u, v1);
        fmpz_set(v1, t1);
    }

    flint_printf("\nnombre d'itérations:%d \n\n", nb_steps);

    fmpz_set(t1, d);
    fmpz_submul(t1, a, u);
    fmpz_divexact(v, t1, b);

    fmpz_clear(t1);
    fmpz_clear(v1);
    fmpz_clear(t3);
    fmpz_clear(v3);
    fmpz_clear(q);
}