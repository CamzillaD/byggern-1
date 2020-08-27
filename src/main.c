#include <avr/io.h>

/* Internal RC oscillator */
#define F_CPU 8000000UL

/* External quartz oscillator */
/* #define F_CPU 4915200UL */
#include <util/delay.h>

int main(){
    /* Init port */
    DDRB |= (1 << PB0);
    PORTB |= (1 << PB0);

    /* Toggle */
    while(1){
        PORTB &= ~(1 << PB0);
        _delay_ms(1000);

        PORTB |= (1 << PB0);
        _delay_ms(1000);
    }

    return 0;
}
