#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <stdint.h>

// volatile char signal[35] = "=.=.=...===.===.===...=.=.=........";
// volatile uint8_t signal_ctr = 0;

ISR(TIMER1_COMPA_vect)
{
    // if (signal[signal_ctr] == '=')
    // {
    //     PORTD |= _BV(PORTD4);
    // }
    // else if (signal[signal_ctr] == '.')
    // {
    //     PORTD &= ~_BV(PORTD4);
    // }
    // signal_ctr = (signal_ctr + 1) % 35;
    PORTD ^= _BV(PORTD4);
}

int main(void)
{
    cli(); // Make sure interrupts are disabled during setup

    // Power management section
    power_all_disable();             // Disable all modules
    power_timer1_enable();           // Enable the counter 1 module
    set_sleep_mode(SLEEP_MODE_IDLE); // Set the sleep mode to IDLE to keep the timer/counter1 running

    // LED setup section
    DDRD |= _BV(DDD4); // Set DDRB to 1 for the LED

    // Prescaler setup section
    TCCR1B &= ~_BV(CS11);  // Set the prescaler to 256
    TCCR1B &= ~_BV(CS10);  // Set the prescaler to 256
    TCCR1B |= _BV(CS12);   // Set the prescaler to 256
    GTCCR |= _BV(PSRSYNC); // Reset the prescaler

    // CTC setup section
    OCR1A = 0x7A12;        // Set the TOP value to have 500ms time interval
    TCCR1B |= _BV(WGM12);  // Set the mode to CTC
    TIMSK1 |= _BV(OCIE1A); // Set the interrupt mode to Match Compare A

    TCNT1 = 0; // Reset timer before starting the loop for consistent delays

    sei(); // Enable interuptions
    
    while (1)
    {
        // Sleep between every interruption
        sleep_mode();
    }
}