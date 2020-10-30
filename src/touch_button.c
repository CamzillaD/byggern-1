#include "touch_button.h"
#include <avr/io.h>

void touch_button_init(){
    /* Set touch button pins as input */
    DDRD &= ~((1 << PD3) | (1 << PD4));

    /* Disable internal pullups */
    PORTD &= ~((1 << PD3) | (1 << PD4));
}

uint8_t touch_button_read(TouchButton button){
    switch(button){
        case TOUCH_BUTTON_LEFT:
            return PIND & (1 << PD4);

        case TOUCH_BUTTON_RIGHT:
            return PIND & (1 << PD3);
    }
}
