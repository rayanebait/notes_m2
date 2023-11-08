#include <stdio.h>



#define ODD(a) ((a&1) == 0)

static long eucl2(long, long);

static int nb_steps = 0;

int main(int argc, char *argv[]){

    //long a = 0b101000110101;
    //long b = 0b100001111011;

    long a = 810;
    long b = 99;

    long d = eucl2(a,b);

    printf("%ld, %ld\n",a, b);
    printf("%ld\n",d);
    printf("%d\n", nb_steps);
    return 0;
}

static long
eucl2(long a, long b){
    if(ODD(a) == 1 && ODD(b) == 1){
        nb_steps++;
        return 2*eucl2(a/2, b/2);
    }
    else if(ODD(a) == 1){
        nb_steps++;
        return eucl2(a/2, b);
    }
    else if(ODD(b) == 1){
        nb_steps++;
        return eucl2(a, b/2);
    }
    else{
        if(a > b){
            nb_steps++;
            return eucl2(a-b, b);
        }
        else if(a < b){
            nb_steps++;
            return eucl2(a, b-a);
        }
        else{
            nb_steps++;
            return a;
        }
    }
}