#include <stdio.h>

#include "error.h"
#include "fq.h"




int main(int argc, char *argv[]){
    if(argc!=4){
        fprintf(stderr, "Usage:%s <poly1> <poly2> <prime modulus>", argv[0]);
        exit(1);
    }

    int_poly_t f,g,h;

    (void) int_poly_from_str(g, argv[2]);
    (void) int_poly_from_str(h, argv[1]);
    int_poly_init(f, 1);

    int32_t modulus = (int32_t) strtol(argv[3], NULL, 10);

    if(int_polymod_mul(f, g, h, modulus)){
        fprintf(stderr, "%s", error_sys_get());
        return 1;
    }

    int_poly_print_pretty(g);
    printf("\n*\n");
    int_poly_print_pretty(h);
    printf("\n=");
    int_poly_print_pretty(f);
    return 0;


}