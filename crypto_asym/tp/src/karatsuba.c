#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*small*small=big*/
typedef uint32_t small;
typedef uint64_t big;

//static uint64_t MAX64 = -1;
//static uint32_t MAX32 = -1;

#define GET_BIT(a, bit) ((a>>bit) & 1)

static short log_2(const big);
//static void divide(const small, const small, small *, small *);
static big karatsuba(const small, const small, clock_t *,clock_t *, int);

int 
main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s <number1> <number2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    big a = atoi(argv[1]);
    big b = atoi(argv[2]);

    clock_t c1,c2=0;

    big c;
    unsigned long m = 0;
    
    for(int i = 0; i < 10000; i++){
        c = karatsuba(a, b, &c1,&c2, i);
        m+=c2-c1;
    }


    printf("temps = %ld\n",m);


    printf("%ld*%ld = %ld\n",a, b, c);


    



    exit(EXIT_SUCCESS);
}


static short
log_2(const big a)
{
    if (a == 0) return 0;

    short log = 0;
    while ((a >> log) != 1) log++;

    return log;
}




static big
karatsuba(const small a, const small b, clock_t *c1, clock_t *c2, int i)
{
    small ra, rb;
    small qa, qb;

    short loga = log_2(a);
    short logb = log_2(b);


    short k = (loga > logb) ? loga : logb;

    qa = (a>>(k>>1));
    qb = (b>>(k>>1));

    ra = a - (qa<<(k>>1));
    rb = b - (qb<<(k>>1));

    *c1 = clock();

    big qab = qa*qb;
    small rab = ra*rb;

    big c =(qab<<k) + ( (qab + rab - (qa-ra)*(qb-rb)) << (k>>1)) + rab;


    *c2 = clock();
 
    return c;
}