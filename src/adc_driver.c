#include "adc_driver.h"
#include <avr/io.h>
#include <stdio.h>
#define F_CPU 4915200UL
#include <util/delay.h>

volatile uint8_t *ext_ADC = (uint8_t *) 0x1400; // Start address for the  ADC

 

void adc_driver_init(){

    // Clear Timer/Counter prescaler
    SFIOR |= (1 << PSR310);

    // ADC frequency half of uC frequency
    OCR0 = 0x00;

    TCCR0 |= (1 << WGM01) | (1 << COM00) | (1 << CS00);

    // Drive ADC clock on PB0
    DDRB |= (1 << PB0);
    PORTB |= (1 << PB0);


    // Manually control ACD chip select
    DDRB |= (1 << PB2);
    PORTB |= (1 << PB2);

    /*
    // Configure ADC channel 0
    PORTB &= ~(1 << PB2);
    *ext_ADC = 0x09;
    PORTB |= (1 << PB2);
    _delay_ms(50);
    */

   DDRB &= ~(1 << PB1);

   // Configure ADC channels 0-3
   PORTB &= ~(1 << PB2); // Assert ADC /CS
   *ext_ADC = 0x80;
   _delay_ms(2);
   *ext_ADC = 0x81;
    _delay_ms(2);

   *ext_ADC = 0x82;
   _delay_ms(2);

   *ext_ADC = 0x83;
   _delay_ms(2);

   PORTB |= (1 << PB2); // Negate ADC /CS

}

uint8_t adc_driver_busy() {
    return !(PINB & (1 << PB1));
}

uint8_t adc_driver_test(){

    // Begin conversion
    PORTB &= ~(1 << PB2);
    *ext_ADC = 0x01;  

    PORTB |= (1 << PB2);
    _delay_ms(1);

    // Read ADC_RAM
    PORTB &= ~(1 << PB2);
    uint8_t sample = *ext_ADC;


    PORTB |= (1 << PB2);
    _delay_ms(1);

    return sample;
    
    }

uint8_t adc_driver_testA1(){

    PORTB &= ~(1 << PB2);
    uint8_t sample = *ext_ADC;


    PORTB |= (1 << PB2);
    _delay_ms(1);

    return sample;
    
    }

uint8_t adc_driver_test_sliders(){

     // Begin conversion
    //PORTB &= ~(1 << PB2);
    //*ext_ADC = 0x82;  

    //PORTB |= (1 << PB2);
    //_delay_ms(1);


    PORTB &= ~(1 << PB2);
    uint8_t sample = *ext_ADC;


    PORTB |= (1 << PB2);
    _delay_ms(1);

    return sample;
    
    }