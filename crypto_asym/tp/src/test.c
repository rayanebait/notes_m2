#include <time.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#define big uint64_t

int main(int argc, char *argv[]){

    big a = atoi(argv[1]);
    big b = atoi(argv[2]);

    clock_t c1 = clock();

    big c;
    
    for(int i = 0; i < 10000; i++){
        c = a*b;
    }

    clock_t c2 = clock();

    unsigned long m = c2-c1;
    printf("temps = %ld\n",m);


    printf("%ld*%ld = %ld\n",a, b, c);

    return 0;
}