#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static short log_2(const long);
static long isqrt(const long , const short);

int main(int argc, char *argv[])
{
    if(argc != 2){
        fprintf(stderr, "Usage: %s <number>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    long a = atoi(argv[1]);

    short loga = log_2(a);

    long iroota = isqrt(a, loga);

    printf("%ld\n" ,iroota);

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

static long
isqrt(const long a, const short loga)
{
    long iroot = 1<<(loga/2);

    int ind = loga/2 - 1;

    while(ind >= 0){
        while((iroot + (1<<ind))*(iroot + (1<<ind)) > a && ind>=0){
            ind--;
        }
        if(ind != -1){
            iroot+=(1<<ind);
        }
    }

    return iroot;
}
