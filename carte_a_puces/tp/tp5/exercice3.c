#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <stdio.h>

#include "wdt_setup.h"
#include "uart.h"


// ISR(PCINT0_vect){
//     PORTD ^= _BV(PORTD5);
// }


uint8_t is_pushd, state, ctr;


void debounce(uint8_t *is_pushed, uint8_t 
            *button_state, uint8_t *counter){
    /*Read if PB5 has high or low logical state*/
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        /*Reading PIN is doable independently
        of DDR and PORT value*/
        uint8_t curr_button_state = (PINB >> PINB5) & 1;

        printf("%d",curr_button_state);

        if(curr_button_state != *button_state){
            *counter++;
            if(*counter >=4){

                *button_state = curr_button_state;
                *counter = 0;

                /*Low logical state -> button is pushed*/
                if(*button_state == 0){
                    *is_pushed = 1;
                }
            }
        } else {
            *counter = 0;
        }
    }
}

void setup_config(){
    set_sleep_mode(SLEEP_MODE_IDLE); // Set the sleep mode to IDLE to keep the Timer1 running

    UART__init(9600);

    /*Set pull up resistor*/
    DDRB |= _BV(DDB5);
    PORTB |= _BV(PORTB5);
    /*Set input pin*/
    DDRB &= ~_BV(DDB5);

    DDRD |= _BV(DDD5);

    // /*Enable pin change interrupt on PCINT5*/
    // PCICR = _BV(PCIE0);
    // PCMSK0 = _BV(PCINT5);
    state = 0;
    ctr = 0;
    is_pushd = 0;

    WDT_timeout_setup(0);
}

/*No need for ADC !*/
int main(void){
    setup_config();

    sei();

    while(1){
        if( is_pushd ){
            PORTD ^= _BV(PORTD5);
            is_pushd = 0;
        }
        sleep_mode();
        debounce(&is_pushd, &state, &ctr);
    }
}