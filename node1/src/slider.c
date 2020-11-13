#include "slider.h"
#include "adc.h"

void slider_read(uint8_t * p_left, uint8_t * p_right){
    *p_left = adc_read(ADC_CHANNEL_2);
    *p_right = adc_read(ADC_CHANNEL_3);
}