#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <stdio.h>

#include "wdt_setup.h"
#include "uart.h"


ISR(WDT_vect){
    PORTD ^= _BV(PORTD5);
}



typedef struct{
    uint8_t state;
    uint8_t counter;
    uint8_t is_pushed;
}button;

typedef button button_t[1];

button_t b;


void debounce(button_t B){
    /*Read if PB5 has high or low logical state*/
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        /*Reading PIN is doable independently
        of DDR and PORT value*/
        uint8_t curr_button_state = (PINB >> PINB5) & 1;

        // printf("%d",curr_button_state);

        if(curr_button_state != B->state){
            B->counter++;
            if(B->counter >=4){

                B->state = curr_button_state;
                printf("%d",B->state);
                B->counter = 0;

                /*Low logical state -> button is pushed*/
                if(B->state == 0){
                    B->is_pushed = 1;
                }
            }
        } else {
            B->counter = 0;
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

    b->state = 1;
    b->is_pushed = 0;
    b->counter = 0;

    WDT_timeout_setup(0);
}

/*No need for ADC !*/
int main(void){
    setup_config();

    sei();

    while(1){
        if( b->is_pushed ){
            PORTD ^= _BV(PORTD5);
            b->is_pushed = 0;
        }
        sleep_mode();
        debounce(b);
    }
}