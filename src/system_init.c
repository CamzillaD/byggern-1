#include "system_init.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void system_pre_init(){
    cli();

    /* System clock prescaler of 1 */
    CLKPR = (1 << CLKPCE);

    /* Enable external memory interface */
    MCUCR |= (1 << SRE);

    /* Mask out JTAG pins from external address lines */
    SFIOR |= (1 << XMM2);
}

void system_post_init(){
    sei();
}
