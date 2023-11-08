#include <avr/io.h>
#include <avr/interrupt.h>


/*Definitions des noms pour acceder aux registres decrites dans /usr/lib/avr/include/avr/iom328p.h*/
void WriteTCNT1( unsigned int i )
{
    unsigned char sreg;
    /*Save GEI*/
    sreg = SREG;
    /*Non atomic operation -> disable interrupts*/
    cli();
    
    TCNT1 = i;

    /*Restore GEI*/
    SREG = sreg;
}

ISR(TIMER1_OVF_vect)
{
    PORTB ^= _BV(PORTB5);
    WriteTCNT1(34286);
}

int main (void)
{
    DDRB |= _BV(DDB5);

    /*Set timer overflow interrupt flag */
    /*Timer overflow interrupt enable*/
    TIMSK1 |= (1<<TOIE1);

    /*/1024*/
    TCCR1B |= (1<<CS12);

    /*(2**16-3036)/(16*(10**6)/256)=1*/
    /*31250/(16*(10**6)/256)=0.5*/
    /*65536-31250=34286*/
    TCNT1 = 34286; 

    sei();
    while(1){
        ;
    }
}
