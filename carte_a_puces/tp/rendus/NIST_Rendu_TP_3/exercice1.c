#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define BLINK_DELAY_MS 500

int main(void)
{
    char signal[35] = "=.=.=...===.===.===...=.=.=........";
    DDRB |= _BV(DDB5);
    while (1)
    {
        for (uint8_t i = 0; i < 35; i++)
        {
            if (signal[i] == '=')
            {
                PORTB |= _BV(PORTB5);      // Turn the LED on
                _delay_ms(BLINK_DELAY_MS); // Wait 500ms
            }
            else if (signal[i] == '.')
            {
                PORTB &= ~_BV(PORTB5);     // Turn the LED off
                _delay_ms(BLINK_DELAY_MS); // Wait 500ms
            }
        }
    }
}