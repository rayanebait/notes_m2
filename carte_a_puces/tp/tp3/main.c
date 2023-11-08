#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 500

int main (void)
{
    DDRB |= _BV(DDB5);
	PORTB &= ~_BV(PORTB5);

	/*PGM_P = const char* et char = PROGMEM prog_char */
	PGM_P str = "=.=.=...==.....==.=...=.===.===...=.=.=........";
	
	/*PROGMEM prog_uint8*/
	uint8_t i = 0;

    while(1) {
		switch(str[i]){
			case '\0':  i = 0;
						continue;
			case '=':   PORTB |= _BV(PORTB5);
						_delay_ms(BLINK_DELAY_MS);
						break;
			case '.':   PORTB &= ~_BV(PORTB5);
						_delay_ms(BLINK_DELAY_MS);
						break;
		}
		i=i+1;
    }
}
