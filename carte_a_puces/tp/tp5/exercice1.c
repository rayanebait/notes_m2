#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <stdio.h>

int i = 0;

ISR(TIMER0_OVF_vect)
{
    // toggle the LED
    OCR0A++;
}

int main(void)
{
    set_sleep_mode(SLEEP_MODE_IDLE); // Set the sleep mode to IDLE to keep the Timer1 running

    DDRD |= _BV(DDD6);

    // LED setup section

    OCR0A = 0x00;
    // Timer1 setup section
    TCCR0A = _BV(WGM01) | _BV(WGM00) | _BV(COM0A1);
    TCCR0B = _BV(CS01) | _BV(CS00); // Set the prescaler to 256 and the mode to CTC
    TIMSK0 = _BV(TOIE0);            // Set the interrupt mode to Match Compare A

    TCNT0 = 0; // Reset timer before starting the loop for consistent delays

    sei(); // Enable interuptions

    while (1)
    {
        // Sleep between every interruption
        sleep_mode();
    }
}