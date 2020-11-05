#include "timer.h"

uint8_t timer_pwm_init(){

    // Enable Clock for TC0 in PMC
    PMC->PMC_WPMR &= ~(1);
	PMC->PMC_PCR = PMC_PCR_EN | (0 << PMC_PCR_DIV_Pos) | PMC_PCR_CMD | (ID_TC0 << PMC_PCR_PID_Pos);
	PMC->PMC_PCER0 |= 1 << (ID_TC0); // Timer counter 0 (TC0)

    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN; 
    TC0->TC_CHANNEL[0].TC_CCR &= ~(TC_CCR_CLKDIS);
    TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG;

    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4; //?????!

    //TC0->TC_CHANNEL[0].TC_IMR = TC_IMR_CPAS; // Enables RA compare interrupt! 

    /* PWM enableings*/
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET;
    TC0->TC_CHANNEL[0].TC_RC = 840000; // Set inn det som vil gi 20ms
    //0x3345


    TC0->TC_CHANNEL[0].TC_RA = 88200;
    //0x1622

    PMC->PMC_PCK[0] |= PMC_PCK_PRES_CLK_1; //Hva gjør egentlig dette?

    PIOB->PIO_PDR |= PIO_PDR_P25; //Setting PB25 (PWM2) to output
    PIOB->PIO_ABSR |= PIO_ABSR_P25;

    return 0;
}

uint8_t timer_set_duty_cycle(double percentage){ 

    uint32_t system_input = (percentage * (84000-42000)) + 42000; //[0,1]

    if(system_input > 84000){
        system_input = 84000;
    }
    else if( system_input < 42000){
        system_input = 42000;
    }

    TC0->TC_CHANNEL[0].TC_RA = system_input;

    return 0;
}


uint32_t timer_read_cv(){

    return TC0->TC_CHANNEL[0].TC_CV;
}