#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// 50ms resolution
#define BLINK_RESOLUTION 12500

ISR(TIMER1_COMPA_vect) {
    // Enable ADC
    ADCSRA |= _BV(ADEN);
    // Start conversion
    ADCSRA |= _BV(ADSC);

    // Wait for a result
    // This is not a good design: we should not wait
    // for something in a ISR :)
    loop_until_bit_is_clear(ADCSRA, ADSC);

    // Get result and store it into OCROA
    // which will change the duty cycle of the PWM output
    OCR0A = ADCH;

    // Poweroff ADC
    ADCSRA &= ~_BV(ADEN);
}

int main (void)
{
    /* Configure LED and PWM */
    // Output mode for PD6
    DDRD |= _BV(DDD6);
    // Clear PD6
    PORTD &= ~_BV(PORTD6);
    // non inverting Fast PWM mode for COMPA
    TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
    // No prescaler
    TCCR0B = _BV(CS00);
    // Counter resolution for pwm
    OCR0A = 0;

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
