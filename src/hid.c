#include "hid.h"
#include "adc.h"
#include <avr/io.h>

void hid_init(){
    /* Use PD4 for left button and PD5 for right button */
    DDRD &= ~((1 << PD4) | (1 << PD5));

    /* Initialize underlying ADC resource */
    adc_init();
}

HidJoystick hid_joystick_read(){
    HidJoystick v;
    v.y = adc_read(ADC_CHANNEL_0);
    v.x = adc_read(ADC_CHANNEL_1);
    return v;
}

HidSlider hid_slider_read(){
    HidSlider v;
    v.left = adc_read(ADC_CHANNEL_2);
    v.right = adc_read(ADC_CHANNEL_3);
    return v;
}

HidButton hid_button_read(){
    HidButton v;
    v.left = ((PIND & (1 << PD4)) != 0);
    v.right = ((PIND & (1 << PD5)) != 0);
    return v;
}
