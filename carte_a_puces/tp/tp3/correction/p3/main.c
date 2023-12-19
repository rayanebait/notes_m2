#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define COUNTER_TOP 31249

volatile char blink_pattern[] = "10101000111011101110001010100000000";
volatile uint8_t counter = 0;

ISR(TIMER1_COMPA_vect) {
    if (blink_pattern[counter] == '1')
    {
        PORTB |= _BV(PORTB5);
    }
    else
    {
        PORTB &= ~_BV(PORTB5);
    }

    // Increment counter
    counter++;
    if (counter >= sizeof(blink_pattern)) {
        counter = 0;
    }
}

int main(void)
{
    // Output mode for PB5
    DDRB |= _BV(DDB5);
    // Clear PB5
    PORTB &= ~_BV(PORTB5);

    // Counter resolution
    OCR1A = COUNTER_TOP;

    // Enable COMPA interrupt
    TIMSK1 |= _BV(OCIE1A);

    // Mode CTC with /256 prescaler
    TCCR1B |= _BV(WGM12) | _BV(CS12);

    // Poweroff ADC
    ADCSRA &= ~_BV(ADEN);

    // Global Interrupt enable
    sei();

    while(1) {
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
    }
}
