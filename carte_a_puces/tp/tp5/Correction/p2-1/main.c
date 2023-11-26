#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

int main (void)
{
    // Output mode for PD6
    DDRD |= _BV(DDD6);
    // Clear PD6
    PORTD &= ~_BV(PORTD6);

    // non inverting Fast PWM mode for COMPA
    TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
    // No prescaler
    TCCR0B = _BV(CS00);

    // Counter resolution for pwm
    // 255 / 10
    OCR0A = 25;

    // Poweroff ADC
    ADCSRA &= ~_BV(ADEN);

    while(1) {
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
    }
}
