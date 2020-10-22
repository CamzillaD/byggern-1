#include "timer.h"


volatile uint8_t timer_interrupt_check; //variable to check if timer_interrrupt has been triggered
volatile uint8_t ITR_interrupt_check; //variable to check if IR-interrupt has been triggered



uint8_t timer_init(){

    // Enable Clock for TC0 in PMC
	PMC->PMC_PCR = PMC_PCR_EN | (0 << PMC_PCR_DIV_Pos) | PMC_PCR_CMD | (ID_TC0 << PMC_PCR_PID_Pos);
	PMC->PMC_PCER0 |= 1 << (ID_TC0); // Timer counter 0 (TC0)

    
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4;

    TC0->TC_CHANNEL[0].TC_IMR = TC_IMR_CPAS; // Enables RA compare interrupt!

    /* PWM enableings*/
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET;

    TC0->TC_CHANNEL[0].TC_RC = 0x3345; // Set inn det som vil gi 20ms


    TC0->TC_CHANNEL[0].TC_CCR = 0x01;

    return 0;
}


uint8_t timer_enable(){
    return 0;
}

uint8_t timer_disable(){
    return 0;
}

uint32_t timer_read_cv(){

    return TC0->TC_CHANNEL[0].TC_CV;
}