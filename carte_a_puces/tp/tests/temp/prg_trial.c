#include <avr/io.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <util/delay.h>

#include "weak_prg.h"
#include "wdt_setup.h"
#include "uart.h"


EMPTY_INTERRUPT(WDT_vect);

void setup(){
    set_sleep_mode(SLEEP_MODE_IDLE);

    UART__init(9600);

    WDT_init();
    WDT_timeout_setup(0);
    
    DDRB &= ~_BV(DDB5);
}

int main(void){
    setup();
    sei();

    printf("MCUSR: %d\r\n", MCUSR);

    while(1){
        if(uECC_rng(seed, SEED_SIZE)){
            printf("hex_seed: ");
            for(uint8_t i = 0; i < SEED_SIZE - 1; i++){
                printf("%.2x:", seed[i]);
            }
            printf("%.2x / raw_seed: ", seed[SEED_SIZE - 1]);
            fwrite(seed, SEED_SIZE, 1, stdout);
            printf("\r\n");
        } else {
            printf("Seed generation failed\r\n");
        }
        sleep_mode();
    }
}