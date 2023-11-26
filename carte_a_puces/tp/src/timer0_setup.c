#include <avr/io.h>

#include "timer0_setup.h"
//TIFR0 to check interrupt flags

void Timer0_Overflow_setup(uint8_t prescaler){
    TCCR0A = 0x00;

    if(prescaler > 5){
        /*no prescaler*/
        TCCR0B = _BV(CS00);
    } else {
        /*prescaler of 1<<2*prescaler*/
        TCCR0B = (prescaler);
    }

    TIMSK0 = _BV(TOIE0);
}
void Timer0_Fast_PWM_setup(uint8_t prescaler){

    /*
        OC0A = readable as OCR0A at pin PD6=6
        (COM0A1) OC0A set on compare match A (readable on PD6(pin 6 on arduino atmega328p))
        +
        (WGM01 + WGM00) Top=Oxff=MAX, TOV Flag set at MAX
    */

    TCCR0A = _BV(WGM01) | _BV(WGM00) | _BV(COM0A1);

    if(prescaler > 5){
        /*No prescaler*/
        TCCR0B = _BV(CS00);
    }
    else{
        TCCR0B = (prescaler);
    }
    /*Set a prescaler of 1<<2*prescaler*/

    /*Overflow interrupt enable*/
    TIMSK0 = _BV(TOIE0);
}

void Timer0_Stop(){
    TCCR0B = 0x00;
}