#include "adc.h"
#include <avr/interrupt.h>

#define F_CPU 16000000ul
#include <util/delay.h>

#ifdef ADC_ACCELEROMETER_ENABLE
#define ADC_NUMBER_OF_CHANNELS 4
#else
#define ADC_NUMBER_OF_CHANNELS 7
#endif

#define ADC_MEM ((volatile uint8_t *)0x1800)
#define ADC_BASE_ADDRESS 0x1800
static uint8_t m_adc_channels[ADC_NUMBER_OF_CHANNELS];


ISR(TIMER2_COMP_vect){
    volatile uint8_t * p_ch;

    for(uint8_t ch = 0; ch < ADC_NUMBER_OF_CHANNELS; ch++){
        p_ch = (volatile uint8_t *)(ADC_BASE_ADDRESS | (ch << 8));

        *p_ch = 0x00;
        _delay_us(1);

        m_adc_channels[ch] = *p_ch;
        _delay_us(1);
    }
}

void adc_init(){
    /* Reset Timer/Counter2 prescaler */
    SFIOR |= (1 << PSR2);

    /* Clock Timer/Counter2 from CPU clock */
    ASSR &= ~(1 << AS2);

    /* Clear timer on compare, disconnect from pins, 1024 prescaler */
    TCCR2 = (1 << WGM21) | (1 << CS22) | (1 << CS21) | (1 << CS20);

    /* ~10 ms period; f_oc = f_clk / (2 * N * (1 + OCR2)) with N = 1024 */
    OCR2 = 78;

    /* Generate interrupt on timer compare match */
    TIMSK |= (1 << OCIE2);
}

uint8_t adc_read(uint8_t adc_channel){
    return m_adc_channels[adc_channel];
}
