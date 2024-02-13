#include "shanks_tonelli.h"
#include <fmpz.h>



void
bin_exp_mod(fmpz_t y, fmpz_t x, fmpz_t n, fmpz_t p)
{
    fmpz_set_ui(y,1);
    if(n==0){
        return;
    }

    fmpz_t N;
    fmpz_init_set(N, n);

    
    while(n!=0)
    {
        if(fmpz_is_odd(N)){
            fmpz_mul(y, y, x);
            fmpz_mod(y, y, p);
        }
        fmpz_fdiv_q_2exp(N, N, 1);
        if(fmpz_is_zero(N)){
            return;
        }
        fmpz_powm_ui(y,y,2,p);
    }
}

void
shanks_tonelli(fmpz_t x, fmpz_t a, fmpz_t p)
{
    if(fmpz_jacobi(a, p)==-1){
        fmpz_set_si(x,-1);
        fmpz_print(a);
        flint_printf(" is not a square mod p\n");
        return;
    }

    fmpz_sub_ui(p, p ,1);
    /*p-1=2^alpha*s*/
    ulong alpha = (ulong) fmpz_val2(p);

    fmpz_t s;
    fmpz_init(s);

    fmpz_fdiv_q_2exp(s,p,alpha);
    fmpz_add_ui(p,p,1);

    /*|y| divides 2^alpha*/
    fmpz_t y;
    fmpz_init_set(y, a);
    fmpz_powm(y, a, s, p);

    /*find non residue mod p*/
    fmpz_t b;
    fmpz_init_set_ui(b, 2);
    while(fmpz_jacobi(b, p)!=-1){
        fmpz_add_ui(b, b, 1);
    };

    /*|b^s|=2^alpha since b is not a square mod p*/
    fmpz_powm(b, b, s, p);

    fmpz_t j;
    fmpz_init_set_ui(j, 0);

    if(alpha<=2){
        /*TODO*/
        flint_printf("alpha<=2");
        fmpz_set(x,b);
        fmpz_clear(s);
        fmpz_clear(b);
        fmpz_clear(y);
        fmpz_clear(j);
        return;
    }

    fmpz_t b_l, tmp_l, u, power;
    fmpz_init(u);
    fmpz_init(tmp_l);
    fmpz_init(b_l);
    fmpz_init(power);

    fmpz_setbit(power, alpha-1);
    fmpz_powm(u, b, power, p);
    fmpz_powm_ui(b_l, b, 2, p);

    if(fmpz_cmp_ui(u, 1)== 0){
        /*if (r^2a^{-1})^(2^(alpha-1))=y^{2^(alpha-1)}=1 -> j_0=0*/
        fmpz_one(b_l);
        fmpz_setbit(j, 0);
        fmpz_set(tmp_l, y);
    } else {
        fmpz_mul(tmp_l, b_l, y);
        fmpz_mod(tmp_l, tmp_l, p);
    }

    for(ulong l=3; l<alpha; l++){

        /*power=2^{alpha-i}*/
        fmpz_zero(power);
        fmpz_setbit(power, alpha-l);
        fmpz_powm(u, tmp_l, power, p);

        /*b_l=b^{2^{l-2}}*/
        fmpz_powm_ui(b_l, b_l, 2, p);

        if(fmpz_cmp_ui(u,1)!=0)
        {
            /*j+=2^{i-2}*/
            fmpz_setbit(j, l-2);

            /*tmp_l=y*b_l*/
            fmpz_mul(tmp_l, b_l, tmp_l);
            fmpz_mod(tmp_l, tmp_l, p);
        }
        

        /*j contains the last power of 2 times j_i*/
        /*set y<-y*b^2*j*/
        // fmpz_powm(v, b, j_l, p);
        // fmpz_powm_ui(v, v, 2, p);

        // fmpz_mul(y, v, y);
        // fmpz_mod(y, y, p);

        // fmpz_setbit(ind, alpha-i);
        // fmpz_powm(u, y, ind, p);

        // fmpz_zero(ind);
        // fmpz_setbit(ind, alpha-i+1);

        // /*b^(2^(l)j_l)**/
        // fmpz_powm(v, b, j);
        // fmpz_invmod(v, v, p);


        

    }

    fmpz_add_ui(s, s, 1);
    fmpz_fdiv_q_2exp(s, s, 1);
    fmpz_powm(u, a, s, p);
    fmpz_powm(x, b, j ,p);
    fmpz_mul(x, x, u);
    fmpz_mod(x, x, p);

    fmpz_clear(s);
    fmpz_clear(y);
    fmpz_clear(b);
    fmpz_clear(j);
    fmpz_clear(tmp_l);
    fmpz_clear(b_l);
    fmpz_clear(power);
    fmpz_clear(u);
}