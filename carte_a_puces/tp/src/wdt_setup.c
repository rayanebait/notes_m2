#include <avr/io.h>
#include <avr/wdt.h>
#include <stdint.h>

#include "wdt_setup.h"


void WDT_init(void){
    MCUSR &= ~_BV(WDRF);                 // Clear the WDT reset flag
    WDTCSR &= ~_BV(WDE);
    WDTCSR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
    WDTCSR = 0x00;                      // Disable the WDT
}

/*For timeouts only, no system resets,
prescaler value=n implies 
(16)*2^(1+n)ms*/
void WDT_timeout_setup(uint8_t prescaler){
    /*Setup part*/


    if(prescaler > 9){
        /*Timed sequence, enable WDE and WDCE*/
        WDTCSR |= _BV(WDCE) | _BV(WDE);
        /* put interrupt enable with no prescaler*/
        WDTCSR = _BV(WDIE);
    } else {
        /*Timed sequence, enable WDE and WDCE*/
        WDTCSR |= _BV(WDCE) | _BV(WDE);
        /* put interrupt enable with prescaler of 1<<2^(1+prescaler)*/
        WDTCSR = _BV(WDIE);
    }
}

void WDT_stop(void){
    MCUSR &= ~_BV(WDRF);                 // Clear the WDT reset flag
    WDTCSR &= ~_BV(WDE);

    WDTCSR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
    WDTCSR = 0x00;                      // Disable the WDT
}
