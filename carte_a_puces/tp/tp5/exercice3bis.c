#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <stdio.h>

#include "wdt_setup.h"
#include "uart.h"


// ISR(WDT_vect){
//     PORTD ^= _BV(PORTD5);
// }

ISR(PCINT0_vect){
    if(!WDTCSR){
        WDT_stop()
    }
    else{
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
            WDT_timeout_setup(4);
        }
    }
    PORTD ^= _BV(PORTD5);
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