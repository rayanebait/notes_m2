#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 500

const char pattern[] = "=.=.=...===.===.===...=.=.=........";

int main (void)
{
    DDRB |= _BV(DDB5);

    uint8_t count = 0;
    while(1) {
        if (pattern[count] == '=') {
            PORTB |= _BV(PORTB5);
        }
        else {
            PORTB &= ~_BV(PORTB5);
        }

        count++;
        if (count == sizeof(pattern)) {
            count = 0;
        }
        _delay_ms(BLINK_DELAY_MS);
    }
}
