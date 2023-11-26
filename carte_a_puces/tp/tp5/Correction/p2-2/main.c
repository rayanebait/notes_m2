#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// Whether to increment or to decrement OCROA
// OCROA is incremented from 0 until its value is equal to 255
// OCROA is then decremented until its value is 0
volatile uint8_t incr = 1;

ISR(TIMER0_OVF_vect) {
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

    // non inverting Fast PWM mode for COMPA
    TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
    // Prescaler /256
    TCCR0B = _BV(CS02);

    // Counter resolution for pwm
    OCR0A = 0;

    // Enable TOV0 interrupt
    TIMSK0 |= _BV(TOIE0);

    // Poweroff ADC
    ADCSRA &= ~_BV(ADEN);

    // Global Interrupt enable
    sei();

    while(1) {
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();

    }
}
