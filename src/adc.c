#include "adc.h"
#include <avr/io.h>

#define ADC_MEM ((volatile uint8_t *)0x1400)

static uint8_t m_adc_channels[4];

static uint8_t adc_busy(){
    return !(PINB & (1 << PB1));
}

void adc_init(){
    // Clear Timer/Counter prescaler
    SFIOR |= (1 << PSR310);

    // ADC frequency half of uC frequency
    OCR0 = 0x00;

    TCCR0 |= (1 << WGM01) | (1 << COM00) | (1 << CS00);

    // Drive ADC clock on PB0
    DDRB |= (1 << PB0);
    PORTB |= (1 << PB0);

    // Sample ADC busy signal on PB1
    DDRB &= ~(1 << PB1);
}

void adc_sample(){
    // Strobe ~WR to start conversion
    *ADC_MEM = 0x00;

    while(adc_busy());

    for(uint8_t c = 0; c < 4; c++){
        m_adc_channels[c] = *ADC_MEM;
    }
}

uint8_t adc_read(ADCChannel channel){
    return m_adc_channels[channel];
}