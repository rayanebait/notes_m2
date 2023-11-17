#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <stdint.h>

// volatile char signal[35] = "=.=.=...===.===.===...=.=.=........";
// volatile uint8_t signal_ctr = 0;

ISR(TIMER1_COMPA_vect)
{
}

int main(void)
{
    cli(); // Make sure interrupts are disabled during setup

    set_sleep_mode(SLEEP_MODE_IDLE); // Set the sleep mode to IDLE to keep the timer/counter1 running

    // LED setup section
    DDRD |= _BV(DDD6); // Set DDRB to 1 for the LED

    // Prescaler setup section

    OCR0A = 0x80;        // Set Output compare to have 0.25 duty cycle

    TCCR0A = _BV(WGM01) | _BV(WGM00);  // Set the mode to Fast PWM
    TCCR0B = _BV(WGM02) | _BV(CS01) | _BV(CS00);//

    TIMSK0 |= _BV(OCIE0A); // Set the interrupt mode to Match Compare A

    TCNT0 = 0; // Reset timer before starting the loop for consistent delays

    sei(); // Enable interuptions
    
    while (1)
    {
        // // Sleep between every interruption
        // sleep_mode();
    }
}