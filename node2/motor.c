#include "motor.h"


void motor_dac_init(){

    PMC->PMC_PCER1 = PMC_PCER1_PID38;
    DACC->DACC_WPMR &= ~(0x00000001);
    
    DACC->DACC_MR = 




    DACC->DACC_WPMR = DACC_WPMR_WPEN;

}