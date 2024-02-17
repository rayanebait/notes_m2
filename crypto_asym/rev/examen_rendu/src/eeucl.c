#include <gmp.h>



int main(int argc, char *argv[]){
    if(argc != 3){
        flint_printf("Usage:\n\t -Input: <a>, <b>\n\t -Output: (u,v,d) with au+bv=d\n");
        exit(1);
    }

    fmpz_t a,b,u,v,d;
    fmpz_init(a);
    fmpz_init(b);
    fmpz_init(u);
    fmpz_init(v);
    fmpz_init(d);
    
    if(fmpz_set_str(a, argv[1], 10) == -1 || fmpz_set_str(b, argv[2], 10) == -1){
        fmpz_clear(a);
        fmpz_clear(b);
        fmpz_clear(d);
        exit(1);
    }

    ext_eucl(a,b,u,v,d);

    flint_printf("(");
    fmpz_print(a);
    flint_printf(")\n*\n(");
    fmpz_print(u);
    flint_printf(")\n+\n(");
    fmpz_print(b);
    flint_printf(")\n*\n(");
    fmpz_print(v);
    flint_printf(")\n=");
    fmpz_print(d);
    flint_printf("\n");

    fmpz_clear(a);
    fmpz_clear(b);
    fmpz_clear(u);
    fmpz_clear(v);
    fmpz_clear(d);
    return 0;


}