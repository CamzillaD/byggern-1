#include "ir.h"
#include <stdint.h>
#include "sam.h"

#define IR_THRESHOLD 0x5dc
#define CONSENSUS_THRESHOLD 5
static uint32_t m_ir_consensus = 0;

uint32_t ir_adc_init(){
    PMC->PMC_PCER1 = PMC_PCER1_PID37; //Skrur på corresponding pheriphial clock
    ADC->ADC_MR = ADC_MR_FREERUN | ADC_MR_LOWRES_BITS_10 | ADC_MR_SLEEP_NORMAL;
    ADC->ADC_CR = ADC_CR_SWRST; //resets ADC, begins adc convcertion
    ADC->ADC_CHER = ADC_CHER_CH0;
    ADC->ADC_COR = ADC_CHER_CH0;

    //ADC->ADC_IER=ADC_IER_COMPE; // comparison event interrupt trigger

    PIOA->PIO_PER = PIO_PDR_P16; //enable pin 2


    return 0;  
}


uint8_t ir_beam_broken(){
    
    uint32_t ir_adc = ADC->ADC_CDR[0];

    if (ir_adc < IR_THRESHOLD){
        m_ir_consensus ++;
    }
    else{
        m_ir_consensus = 0;
    }
    
    if(m_ir_consensus >= CONSENSUS_THRESHOLD){
        return 1;
    }
    return 0;   
}


