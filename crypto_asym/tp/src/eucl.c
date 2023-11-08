#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define GET_BIT(a, bit) ((a>>bit) & 1)

static short log_2(const long);
static void divide(const long, const long, long *, long *);


int main(int argc, char *argv[]){

    long a,b;
    a = 38943;
    b = 32;

    printf("%ld, %ld\n",a/b, a%b);

    long q,r;

    divide(a, b, &q, &r);

    printf("%ld, %ld\n", q, r);

    return 0;
}

static short
log_2(const long a)
{
    if (a == 0) return 0;

    short log = 0;
    while ((a >> log) != 1) log++;

    return log;
}


static void
divide(const long a,const long b, long *q, long *r)
{
    *q = 0;
    if( b > a )
    {
        *r = b;
    }
    else
    {
        short log_2b = log_2(b);
        short log_2a = log_2(a);
        short ind = log_2a - log_2b;

        long tmp = a>>(ind - 1);
        tmp = ((tmp>>1) < b) ? tmp : tmp>>1;

        while(ind > 0)
        {
            if(tmp > b){
                tmp -= b;
                *q = *q<<1;
                *q+=1;
            }
            else{
                tmp = tmp<<1;
                tmp += GET_BIT(a, ind);
                *q = *q<<1;
            }

            ind--;
        }
        *r = tmp;
    }
}