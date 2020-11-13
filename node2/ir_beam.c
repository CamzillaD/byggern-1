#include "ir_beam.h"
#include "sam.h"

#define IR_THRESHOLD 0x5dc
#define CONSENSUS_THRESHOLD 5
static uint32_t m_ir_consensus = 0;

void ir_beam_init(){
    /* Enable ADC peripheral clock */
    PMC->PMC_PCER1 = PMC_PCER1_PID37;

    /* Freerunning 10 bit, retain power between conversions */
    ADC->ADC_MR = ADC_MR_FREERUN
                | ADC_MR_LOWRES_BITS_10
                | ADC_MR_SLEEP_NORMAL
                ;

    /* Enable channel 0 */
    ADC->ADC_CHER = ADC_CHER_CH0;

    /* Software reset ADC */
    ADC->ADC_CR = ADC_CR_SWRST;

    //PIOA->PIO_PER = PIO_PDR_P16; //enable pin 2
}


uint8_t ir_beam_broken(){
    uint32_t ir_adc = ADC->ADC_CDR[0];

    if (ir_adc < IR_THRESHOLD){
        m_ir_consensus++;
    }
    else{
        m_ir_consensus = 0;
    }
    
    if(m_ir_consensus >= CONSENSUS_THRESHOLD){
        return 1;
    }
    return 0;   
}


