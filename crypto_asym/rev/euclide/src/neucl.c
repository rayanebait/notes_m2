#include "error.h"
#include "naive_eucl.h"
#include <flint.h>




int main(int argc, char *argv[]){
    if(argc != 3){
        flint_printf("Usage:\n\t -Input: <a>, <b>\n\t -Output: gcd(a,b)\n");
        exit(1);
    }

    fmpz_t a,b,d;
    fmpz_init(a);
    fmpz_init(b);
    fmpz_init(d);
    
    if(fmpz_set_str(a, argv[1], 10) == -1 || fmpz_set_str(b, argv[2], 10) == -1){
        fmpz_clear(a);
        fmpz_clear(b);
        fmpz_clear(d);
        exit(1);
    }

    naive_eucl(a,b,d);
    flint_printf("\ngcd(a,b)=");
    fmpz_print(d);
    flint_printf("\n");

    fmpz_clear(a);
    fmpz_clear(b);
    fmpz_clear(d);
    return 0;


}