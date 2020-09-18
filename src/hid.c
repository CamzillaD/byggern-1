#include "hid.h"
extern volatile char *ext_ADC;
#define F_CPU 4915200UL
#include <util/delay.h>
#include "adc_driver.h"


void hid_buttons_init(){

    DDRD &= ~((1 << PD4) | ( 1 << PD5 )); //Set pins to input

}

uint8_t hid_buttons_read_left(){
    return (PIND & (1 << PD4)) != 0;
}

uint8_t hid_buttons_read_right(){
    return (PIND & (1 << PD5)) != 0;
}


JoystickPosition hid_joystick_read(){

    JoystickPosition pos;

     // Begin conversion
    PORTB &= ~(1 << PB2);
    *ext_ADC = 0x01;  

    PORTB |= (1 << PB2);
    _delay_ms(1);

    // Read ADC_RAM
    PORTB &= ~(1 << PB2);
    pos.y = *ext_ADC;


    PORTB |= (1 << PB2);
    _delay_ms(1);


    // Read ADC_RAM
    PORTB &= ~(1 << PB2);
    pos.x = *ext_ADC;


    PORTB |= (1 << PB2);
    _delay_ms(1);

    return pos;
}


SliderPosition hid_slider_read(){

    SliderPosition slider;


    /*
   // Begin conversion
    PORTB &= ~(1 << PB2);
    *ext_ADC = 0x02;  

    PORTB |= (1 << PB2);
    _delay_ms(1);

    // Read ADC_RAM
    PORTB &= ~(1 << PB2);
    slider.left = *ext_ADC;


    PORTB |= (1 << PB2);
    _delay_ms(1);


    // Read ADC_RAM
    PORTB &= ~(1 << PB2);
    slider.right = *ext_ADC;


    PORTB |= (1 << PB2);
    _delay_ms(1);

    */
   
    PORTB &= ~(1 << PB2);   // Assert ADC /CS
    *ext_ADC = 0x82;        // Start conversion and read from CH2
    PORTB |= (1 << PB2);    // Negate ADC /CS
    //while(adc_busy()){};    // Wait for ADC
    _delay_us(15);

    PORTB &= ~(1 << PB2);
    slider.left = *ext_ADC;
    PORTB |= (1 << PB2);

    PORTB &= ~(1 << PB2);
    *ext_ADC = 0x83;
    PORTB |= (1 << PB2);
    //while(adc_busy()){};
    _delay_us(15);
    
    PORTB &= ~(1 << PB2);
    slider.right = *ext_ADC;
    PORTB |= (1 << PB2);

    return slider;
}