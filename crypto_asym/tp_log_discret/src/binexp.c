#include <stdio.h>

#include "error.h"
#include "fq.h"




int main(int argc, char *argv[]){
    if(argc!=5){
        fprintf(stderr, "Usage:%s <(poly) generator of Fq> <prime modulus> <to power> <power>", argv[0]);
        exit(1);
    }

    int_poly_t modulus,f;

    (void) int_poly_from_str(modulus, argv[1]);
    int32_t prime = (int32_t) strtol(argv[2], NULL, 10);
    (void) int_poly_from_str(f, argv[3]);
    int32_t power = (int32_t) strtol(argv[4], NULL, 10);

    int_poly_t powered;
    int_poly_init(powered, 1);

    if(int_polymod_binexp(modulus, powered, f, power, prime)){
        fprintf(stderr, "%s", error_sys_get());
        return 1;
    }

    printf("prime: %d\n", prime);
    printf("generator:");
    int_poly_print_pretty(modulus);
    printf("\n");
    int_poly_print_pretty(f);
    printf("**%d=", power);
    int_poly_print_pretty(powered);

    int_poly_clear(powered);
    int_poly_clear(f);
    int_poly_clear(modulus);
    return 0;


}