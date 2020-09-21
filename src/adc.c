#include "adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define ADC_MEM ((volatile uint8_t *)0x1400)
#define ADC_NUMBER_OF_CHANNELS 4

static uint8_t m_adc_channels[ADC_NUMBER_OF_CHANNELS];

static uint8_t adc_busy(){
    return !(PINB & (1 << PB1));
}

ISR(TIMER2_COMP_vect){
    /* Strobe ~WR to start conversion */
    *ADC_MEM = 0x00;

    while(adc_busy());

    for(uint8_t c = 0; c < ADC_NUMBER_OF_CHANNELS; c++){
        m_adc_channels[c] = *ADC_MEM;
    }
}

void adc_init(){
    /* Clear Timer/Counter1 and Timer/Counter3 prescaler */
    SFIOR |= (1 << PSR310);

    /* ADC frequency half of uC frequency */
    OCR0 = 0x00;

    /* Clear timer on compare, toggle on compare, no prescaling */
    TCCR0 |= (1 << WGM01) | (1 << COM00) | (1 << CS00);

    /* Drive ADC clock on PB0 */
    DDRB |= (1 << PB0);
    PORTB |= (1 << PB0);

    /* Sample ADC busy signal on PB1 */
    DDRB &= ~(1 << PB1);



    /* --- Configure Timer/Counter2 for interrupt generation --- */

    /* Clear Timer/Counter2 prescaler */
    SFIOR |= (1 << PSR2);

    /* Clock Timer/Counter2 from CPU clock */
    ASSR &= ~(1 << AS2);

    /* Clear timer on compare, disconnect from pins, 1024 prescaler */
    TCCR2 = (1 << WGM21) | (1 << CS22) | (1 << CS21) | (1 << CS20);

    /* 10 ms period; f_oc = f_clk / (2 * N * (1 + OCR2)) with N = 1024 */
    OCR2 = 23;

    /* Generate interrupt on timer compare match */
    TIMSK |= (1 << OCIE2);
}

uint8_t adc_read(ADCChannel channel){
    return m_adc_channels[channel];
}
