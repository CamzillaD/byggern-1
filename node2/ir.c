#include "ir.h"
#include <stdint.h>
#include "sam.h"

uint32_t ir_adc_init(){
    ADC->ADC_CR = ADC_CR_START | ADC_CR_SWRST; //resets ADC, begins adc convcertion
    ADC->ADC_MR = ADC_MR_FREERUN | ADC_MR_LOWRES_BITS_10;
    ADC->ADC_CHER = ADC_CHER_CH0;
    ADC->ADC_COR = ADC_CHER_CH0;
    ADC->ADC_IER=ADC_IER_COMPE; // comparison eeb\\vent interrupt trigger

    PIOA->PIO_PER = PIO_PDR_P2; //hvorfor må vi disable pin p2??? why?? wtf??
}

uint32_t ir_read_signal(){
    return ADC->ADC_CDR[0];