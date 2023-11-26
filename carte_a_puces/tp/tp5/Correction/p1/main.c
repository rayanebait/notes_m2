#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 500

/*
 * U = 5V (entre GPIO et GND)
 * U_led = 2.1V (rouge) (point de fonctionnement LED)
 * i = 20mA (point de fonctionnement LED)
 * U_res = U - U_led = 2.9V
 * R = U_res / i  = 145 Ohms
*/

int main (void)
{
    DDRD |= _BV(DDD6);

    while(1) {
        PORTD |= _BV(PORTD6);
        _delay_ms(BLINK_DELAY_MS);

        PORTD &= ~_BV(PORTD6);
        _delay_ms(BLINK_DELAY_MS);
    }
}
