#include <avr/io.h>
#include <util/atomic.h>

#include "timer1_setup.h"

//TIMER1/0_COMPA/B_vect
//TIMER1/0_OVF_vect

/*16 bit write is not atomic*/
void Write_Timer1( unsigned int i )
{
    /*Ensures atomic write*/
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        TCNT1 = i;
    }
}

void Timer1_overflow_setup(uint8_t prescaler){

    TIMSK1 = (1<<TOIE1);

    /*Prescaler of 1<<2*prescaler*/
    TCCR1B = (1<<prescaler);
}

// void Timer1_PWM_setup(uint8_t prescaler){

// }
