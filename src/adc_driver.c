#include "adc_driver.h"
#include <avr/io.h>
#include <stdio.h>
#define F_CPU 4915200UL
#include <util/delay.h>

volatile uint8_t *ext_ADC = (uint8_t *) 0x1400; // Start address for the  ADC

 

void adc_init(){

    // Clear Timer/Counter prescaler
    SFIOR |= (1 << PSR310);

    // ADC frequency half of uC frequency
    OCR0 = 0x00;
    TCCR0 |= (1 << WGM01) | (1 << COM00) | (1 << CS00);

    // Drive ADC clock on PB0
    PORTB |= (1 << PB0);


    // Manually control ACD chip select
    DDRB |= (1 << PB2);
    PORTB |= (1 << PB2);

    // Configure ADC channel 0
    PORTB &= ~(1 << PB2);
    *ext_ADC = 0x09;
    PORTB |= (1 << PB2);
    _delay_ms(50);

}

uint8_t adc_test(){

    // Begin conversion
    PORTB &= ~(1 << PB2);
    *ext_ADC = 0x01;
    PORTB |= (1 << PB2);
    _delay_ms(1);

    // Read RAM
    PORTB &= ~(1 << PB2);
    uint8_t sample = *ext_ADC;
    PORTB |= (1 << PB2);
    _delay_ms(1);

    return sample;
}