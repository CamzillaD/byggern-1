#include "hid.h"
#include "adc.h"
#include <avr/io.h>

#define HID_JOYSTICK_ACTIVATION 5

static struct {
    uint8_t joy_x_center;
    uint8_t joy_y_center;
    uint8_t joy_x_high;
    uint8_t joy_y_high;
    uint8_t joy_x_low;
    uint8_t joy_y_low;
} m_hid_config;

static inline uint8_t hid_joystick_center(uint8_t x, uint8_t y){
    if(
        x > (m_hid_config.joy_x_center - HID_JOYSTICK_ACTIVATION) &&
        x < (m_hid_config.joy_x_center + HID_JOYSTICK_ACTIVATION) &&
        y > (m_hid_config.joy_y_center - HID_JOYSTICK_ACTIVATION) &&
        y < (m_hid_config.joy_y_center + HID_JOYSTICK_ACTIVATION)
    ){
        return 1;
    }
    return 0;
}

void hid_init(){
    /* Use PD4 for left button and PD5 for right button */
    DDRD &= ~((1 << PD4) | (1 << PD5));

    /* Initialize underlying ADC resource */
    adc_init();

    m_hid_config.joy_x_center = 185;
    m_hid_config.joy_y_center = 189;
    m_hid_config.joy_x_high = 252;
    m_hid_config.joy_y_high = 252;
    m_hid_config.joy_x_low = 15;
    m_hid_config.joy_y_low = 15;
}

HidJoystick hid_joystick_read(){
    HidJoystick v;
    v.position = HID_JOYSTICK_INVALID;

    v.y = adc_read(ADC_CHANNEL_0);
    v.x = adc_read(ADC_CHANNEL_1);

    if(hid_joystick_center(v.x, v.y)){
        v.position = HID_JOYSTICK_CENTER;
    }
    else if(v.y > m_hid_config.joy_y_high){
        v.position = HID_JOYSTICK_UP;
    }
    else if(v.y < m_hid_config.joy_y_low){
        v.position = HID_JOYSTICK_DOWN;
    }
    else if(v.x > m_hid_config.joy_x_high){
        v.position = HID_JOYSTICK_RIGHT;
    }
    else if(v.x < m_hid_config.joy_x_low){
        v.position = HID_JOYSTICK_LEFT;
    }

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
