#include <gmp.h>



int main(int argc, char *argv[]){
    if(argc != 3){
        // flint_printf("Usage:\n\t -Input: <a>, <b>\n\t -Output: (u,v,d) with au+bv=d\n");
        exit(1);
    }

    mpz_t m,b,u,v,d;
    mpz_init(m);
    mpz_init(b);
    mpz_init(u);
    mpz_init(v);
    mpz_init(d);
    
    if(mpz_set_str(m, argv[1], 10) == -1 || mpz_set_str(b, argv[2], 10) == -1){
        mpz_clear(m);
        mpz_clear(b);
        mpz_clear(d);
        exit(1);
    }

    ext_eucl(m,b,u,v,d);

    // flint_printf("(");
    mpz_print(m);
    // flint_printf(")\n*\n(");
    mpz_print(u);
    // flint_printf(")\n+\n(");
    mpz_print(b);
    // flint_printf(")\n*\n(");
    mpz_print(v);
    // flint_printf(")\n=");
    mpz_print(d);
    // flint_printf("\n");

    mpz_clear(m);
    mpz_clear(b);
    mpz_clear(u);
    mpz_clear(v);
    mpz_clear(d);
    return 0;


}