#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef uint16_t block_t;
typedef block_t sbox_t[16];
typedef block_t pbox_t[16];

sbox_t sbox = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7};
pbox_t pbox = {0x0, 0x4, 0x8, 0xc, 0x1, 0x5, 0x9, 0xd, 0x2, 0x6, 0xa, 0xe, 0x3, 0x7, 0xb, 0xf};


block_t heys_perm(block_t partial_ct)
{
    return pbox[partial_ct];
}

block_t heys_subst(block_t partial_ct)
{
    return sbox[partial_ct];
}

block_t xor(block_t a, block_t b)
{
    return a^b;
}

block_t turn_block(block_t partial_ct, block_t partial_key)
{
    partial_ct = heys_subst(partial_ct);
    partial_ct = heys_perm(partial_ct);
    return xor(partial_ct, partial_key);
}

block_t *enc_nturn(block_t pt, block_t *key, size_t n)
{
    if((n<1 || n>1000)|| key == NULL || pt == NULL){
        return NULL;
    }

    block_t *ct = malloc(sizeof(block_t)*(4*n));
    if(!ct){
        return NULL;
    }
    /*memset sets inits raw memory, i.e. bytes. (16 bits=2 bytes)*/
    memset(ct, 0, 2*n);
    block_t u = turn_block(pt, key[0]);


    for(size_t i = 1; i < n ; i++){
            u = turn_block(u, key[i]);
    }

    return u;
}

int main(int argc, char *argv[])
{
    if(argc!=2){
        fprintf(stderr, "Usage: <./SPN_attack> <byte>");
        exit(1);
    }

    if (strlen( (char *) argv[1]) != 1) {
        fprintf(stderr, "single byte");
        exit(1);
    }

    uint8_t byte = (uint8_t) argv[1];
    uint16_t key[5] = {0x4, 0xfe, 0x30, 0x11, 0xa7};

    uint8_t enc = 0;




    exit(0);

}