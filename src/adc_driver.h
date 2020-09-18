#ifndef ADC_DRIVER_H 
#define ADC_DRIVER_H 
#include <stdio.h>

//Access address space 

void adc_driver_init();

uint8_t adc_driver_test();
uint8_t adc_driver_testA1();
uint8_t adc_driver_test_sliders();

uint8_t adc_driver_busy();

#endif


