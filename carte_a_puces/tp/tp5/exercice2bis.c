#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "timer0_setup.h"
#include "adc_setup.h"

ISR(ADC_vect){
    OCR0A = ADCH;
}

ISR(TIMER1_COMPA_vect){


}

void setup_config(){
    timer

}

int main(void){
    power_adc_enable();
    set_sleep_mode(SLEEP_MODE_IDLE); // Set the sleep mode to IDLE to keep the Timer1 running

    /*set pin D6 to output*/
    DDRD |= _BV(DDD6);

    /*MUX1 for input in ADC1, ADLAR to put 8 bits in ADCH*/
    ADMUX = _BV(REFS0) | _BV(ADLAR); 


    /*ADEN enables, ADIE interrupt enables, ADATE for auto trigger ADC*/
    ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADATE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);


    TCCR0A = _BV(WGM01) | _BV(WGM00) | _BV(COM0A1); // Set the mode to Fast PWM with the desired mode for Timer0
    TCCR0B = _BV(CS00); 

    sei();
    /*start conversion*/
    ADCSRA |= _BV(ADSC);
    while(1){
        sleep_mode();
    }
}