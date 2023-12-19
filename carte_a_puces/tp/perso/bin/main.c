#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "timer1_setup.h"
#include "wdt_setup.h"



ISR(TIMER0_OVF_vect)
{
    // toggle the LED
    OCR0A++;
}

void setup(){
    set_sleep_mode(SLEEP_MODE_IDLE);
    Timer0_Fast_PWM_setup(5, 0x80);
    DDRD |= _BV(DDD6);
    OCR0A = 0;
    TCNT0 = 0;
}

int main(){
    setup();

    sei();

    while(1){
        sleep_mode();
    }
}