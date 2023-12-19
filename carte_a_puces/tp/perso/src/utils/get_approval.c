#include "get_approval.h"


#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer1_setup.h"
#include "timer0_setup.h"
#include "error.h"

volatile uint8_t button_pushed = 0;
volatile uint8_t timer_half_seconds = 0;

ISR(TIMER1_COMPA_vect){
    timer_half_seconds++;
    PORTB ^= _BV(PORTB5);
}

ISR(PCINT0_vect){
    PCMSK0 |= _BV(PCINT0);
    button_pushed = 1;
}

status_t get_approval(void){
    button_pushed = 0;
    /*enable on pin PB0=PCINT0*/
    PCMSK0 = _BV(PCINT0);
    /*Pin change interrupt enable*/
    PCICR = _BV(PCIE0);

    /*COMPA = 0x7a12 and prescaler of 256 -> 0.5s delay */
    DDRB |= _BV(PORTB5);

    Timer1_CTCA_setup(0x7a12, 4);
    Write_Timer1(0);

    timer_half_seconds = 0;

    /*For the prg*/
    Timer0_Overflow_No_Interrupt_setup(1);
    while(( timer_half_seconds < 20) && !( button_pushed ));
    

    PCMSK0 |= _BV(PCINT0);

    Timer1_Stop();
    timer_half_seconds = 0;

    if(button_pushed){
        return STATUS_OK;
    }

    return STATUS_ERR_APPROVAL;
}