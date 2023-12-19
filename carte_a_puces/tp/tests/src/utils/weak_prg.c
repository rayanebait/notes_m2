#include <stdint.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/atomic.h>

#include "weak_prg.h"

/*Based on reading a pin that's randomly 
1 or 0. Needs a software interrupt every 16/32ms*/
uint8_t get_byte(){
    uint8_t byte = 0;

    /*Get a bit at every interrupt*/
    for(uint8_t i = 0; i < (uint8_t) 8; i++){
        uint8_t bit = 0;
        /*Wait interrupt*/
        sleep_mode();
        /*Get pin value at PINB5*/
        bit = (PINB >> PINB5) & 1;
        byte |= (bit<<i);
    }

    return byte;
}


/*
Uses a tilt ball switch joining ground and a pin:
    -Enable interrupts every 16/32ms.
    -At startup of gen_seed, shake the tilt ball
    for ~2/4seconds.
*/
void rng(){
    for(uint8_t i = 0; i < SEED_SIZE; i++){
        seed[i] = get_byte();
    }
}

// static int score_seed(){
//     uint16_t seed_score = 0;
//     for(uint8_t i = 0; i < SEED_SIZE;i++){
//         seed_score += ((uint16_t) seed[i]);
//     }

//     return seed_score;
// }

// void gen_seed(){
//     int score;
//     /*Don't proceed until a valid seed is computed*/
//     /*Always ends in one cycle, don't know why*/
//     while(1){
//         rng();
//         score = score_seed();
//         if( score < 500 && 3300 < score){
//             continue;
//         } else {
//             break;
//         }
//     }
// }

int uECC_rng(uint8_t *dest, unsigned size){
    rng();
    int score = 0;
    for(int i = 0; i < SEED_SIZE ; i++){
        score += (int) seed[i];
    }

    if(( score < ((int) SEED_SIZE/5)*255 )
                || ( (SEED_SIZE*255 - ((int) SEED_SIZE/5)*255) < score) ){
            return 0;
    }

    for(uint8_t i = 0; i < size; i++){
        dest[i] = seed[i];
    }

    return 1;
}

