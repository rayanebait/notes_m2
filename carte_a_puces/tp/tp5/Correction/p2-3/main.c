#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BLINK_RESOLUTION 31250

// Whether to increment or to decrement OCROA
// OCROA is incremented from 0 until its value is equal to 255
// OCROA is then decremented until its value is 0
volatile uint8_t incr = 1;

ISR(TIMER1_COMPA_vect) {
    if (incr) {
        OCR0A += 1;
    }
    else {
        OCR0A -= 1;
    }

    if (incr && OCR0A == 255) {
        incr = 0;
    }
    else if (!incr && OCR0A == 0) {
        incr = 1;
    }
}

int main (void)
{
    // Output mode for PD6
    DDRD |= _BV(DDD6);
    // Clear PD6
    PORTD &= ~_BV(PORTD6);

    // Timer 0 => PWM
    // non inverting Fast PWM mode for COMPA
    TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
    // No prescaler
    TCCR0B = _BV(CS00);
    // Counter resolution for pwm
    OCR0A = 0;


    // Timer 1 => execute ISR every 500ms / 256 to update OCR0A
    // Therefore it will take around 500ms for OCR0A to
    // go from 0 to 255
    // Counter resolution
    OCR1A = BLINK_RESOLUTION;
    // Enable COMPA interrupt
    TIMSK1 |= _BV(OCIE1A);
    // Mode CTC with /1 prescaler
    TCCR1B |= _BV(WGM12) | _BV(CS10);

    // Poweroff ADC
    ADCSRA &= ~_BV(ADEN);

    // Global Interrupt enable
    sei();

    while(1) {
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
    }
}
