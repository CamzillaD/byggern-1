#include "servo.h"
#include "sam.h"

void servo_init(){
    /* Enable peripheral clock */
    PMC->PMC_PCER0 = PMC_PCER0_PID27;

    /* Hand PIO to peripheral control */
    PIOB->PIO_PDR = PIO_PDR_P25;
    PIOB->PIO_ABSR = PIO_ABSR_P25;

    /* Enable Timer Counter clock */
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

    /* Master clock / 128 */
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4;

    /* Enable waveform generation */
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE
                              | TC_CMR_WAVSEL_UP_RC
                              | TC_CMR_ACPA_CLEAR
                              | TC_CMR_ACPC_SET
                              ;

    /* 20 ms period */
    TC0->TC_CHANNEL[0].TC_RC = 840000;

    /* 1.5 ms duty */
    TC0->TC_CHANNEL[0].TC_RA = 63000;
}

void servo_command_gimbal(uint8_t gimbal){
    TC0->TC_CHANNEL[0].TC_RA = 42000 + 165 * gimbal;
}