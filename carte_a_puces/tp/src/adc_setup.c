#include <avr/io.h>
#include <stdint.h>

#include "adc_setup.h"

void Adc_8bit_autotrigger_setup(uint8_t prescaler, char interrupt){
    if(prescaler>5){
        Adc_8bit_autotrigger_setup(4, interrupt);
    }
    /*No MUX pin for ADC0, ADLAR to put 8 bits in ADCH*/
    ADMUX = _BV(REFS0) | _BV(ADLAR); 

    /*
        ADEN enables, ADIE interrupt enables,
        ADATE for auto trigger ADC
    */
    switch(interrupt){
        case 'y':
                /*prescaler of value n means prescaler of 1<<n - 1*/
                ADCSRA = _BV(ADEN) | _BV(ADIE)
                    | _BV(ADATE) | prescaler;
            break;
        case 'n':
                ADCSRA = _BV(ADEN) | _BV(ADATE)
                                   | prescaler;
            break;
        default:
            Adc_8bit_autotrigger_setup(prescaler, 'n');
    }
}