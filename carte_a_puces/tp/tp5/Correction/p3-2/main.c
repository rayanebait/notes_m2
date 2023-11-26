#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// 50ms resolution
#define BLINK_RESOLUTION 12500

// We use Counter0 with prescaler /1024.
// A TOV interrupt will be raised every 16ms which is
// too fast for a human eye :)
// We use an "external" counter, `counter`, which will
// be incremented at each TOV0 interrupt, until it
// reaches max_count. At this moment, the LED is toggled
// and `counter` is reset.
// By default, max_count is set to 31 to that the led
// blinks every 16ms*31 ~= 500ms
// max_count is controlled by the potentiometer.
// It varies between 0 and 255, which meens that
// blink duration will vary between 16ms (toggle at each
// TOV) and 16ms*255=4s.

volatile uint8_t counter = 0;
// Defaults to something like 500ms toggle
volatile uint8_t max_count = 31;

ISR(TIMER0_OVF_vect) {
    counter++;

    if (counter >= max_count) {
        counter = 0;
        PORTD ^= _BV(PORTD6);
    }
}

ISR(TIMER1_COMPA_vect) {
    // Enable ADC
    ADCSRA |= _BV(ADEN);
    // Start conversion
    ADCSRA |= _BV(ADSC);

    loop_until_bit_is_clear(ADCSRA, ADSC);

    // Get result
    max_count = ADCH;

    // Poweroff ADC
    ADCSRA &= ~_BV(ADEN);
}

int main (void)
{
    /* Configure LED */
    // Output mode for PD6
    DDRD |= _BV(DDD6);
    // Clear PD6
    PORTD &= ~_BV(PORTD6);

    // Normal mode
    TCCR0A = 0;
    // prescaler /1024
    TCCR0B = _BV(CS02) | _BV(CS00);
    // TOV interrupt
    TIMSK0 = _BV(TOIE0);

    /*
     * Configure Counter1 to check regularly (50ms)
     * the state of the potentiometer
     */
    // Counter resolution
    OCR1A = BLINK_RESOLUTION;
    // Enable COMPA interrupt
    TIMSK1 |= _BV(OCIE1A);
    // Mode CTC with /64 prescaler
    TCCR1B |= _BV(WGM12) | _BV(CS11) | _BV(CS10);


    /*
     * Configure ADC
     */
    // Poweroff ADC to save power
    ADCSRA &= ~_BV(ADEN);

    // Use Vcc as AREF
    // Left align (we only need 8bit resolution)
    // Use ADC0
    ADMUX |= _BV(REFS0) | _BV(ADLAR);
    // /128 prescaler
    ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);

    // Global Interrupt enable
    sei();

    while(1) {
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
    }
}
