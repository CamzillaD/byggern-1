#ifndef ADC_H
#define ADC_H
#include <stdint.h>

typedef enum {
    ADC_CHANNEL_0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3
} ADCChannel;

void adc_init();

void adc_sample();

uint8_t adc_read(ADCChannel channel);

#endif