#include "motor.h"
#include "sam.h"

static inline void m_direction_left(){
    PIOD->PIO_SODR = PIO_SODR_P10;
}

static inline void m_direction_right(){
    PIOD->PIO_CODR = PIO_CODR_P10;
}

void motor_init(){
    /* Initialize DACC */
    PMC->PMC_PCER1 = PMC_PCER1_PID38;
    DACC->DACC_CHER = DACC_CHER_CH1;
    DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL1;

    /* Motor control pins; EN | DIR */
    PIOD->PIO_PER = PIO_PER_P9 | PIO_PER_P10;
    PIOD->PIO_OER = PIO_OER_P9 | PIO_OER_P10;

    /* Enable motor */
    REG_PIOD_SODR = PIO_PD9;
}

void motor_command_speed(uint8_t speed){
    if(speed >= 127 && speed <= 128){
        DACC->DACC_CDR = 0x000;
    }
    else if(speed < 127){
        m_direction_left();
        /* 0xfff - speed * 32 */
        DACC->DACC_CDR = 0xfff - (speed << 5);
    }
    else if(speed > 128){
        m_direction_right();
        /* speed * 32 - 0xfff */
        DACC->DACC_CDR = (speed << 5) - 0xfff;
    }
}