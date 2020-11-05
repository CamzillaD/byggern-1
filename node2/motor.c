#include "motor.h"
#include "uart_and_printf/printf-stdarg.h"

static uint16_t m_encoder_max_value;

void motor_dac_init(){

    PMC->PMC_PCER1 = PMC_PCER1_PID38; //enable clock
    DACC->DACC_WPMR &= ~DACC_WPMR_WPEN;
    DACC->DACC_CHER =DACC_CHER_CH1;
    DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL1;
    DACC->DACC_WPMR = DACC_WPMR_WPEN;

}

void motor_init(){
    REG_PMC_PCER0 = PMC_PCER0_PID14 | PMC_PCER0_PID13 ; //enable clock

 //   REG_PIOC_WPMR = 0x50494f00;
 //   REG_PIOD_WPMR = 0x50494f00;

    uint32_t input_mask = PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P4
                        | PIO_PER_P5 | PIO_PER_P6 | PIO_PER_P7 | PIO_PER_P8;     

    REG_PIOC_PER = input_mask; //enable pc1-pc8 = DO0-D07 => encoder counter value
    REG_PIOC_ODR = input_mask; //output disable = input

    REG_PIOC_PUER = input_mask;

    REG_PIOD_PER = PIO_PER_P0 | PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P9 |  PIO_PER_P10; //Enable pin
    REG_PIOD_OER = PIO_PER_P0 | PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P9 |  PIO_PER_P10; //Enable output

    REG_PIOD_CODR = PIO_ODR_P9; //set enable low
    REG_PIOD_SODR = PIO_SODR_P0 | PIO_SODR_P1; //restes, and OE pin high, no value on MJ2-pins

    REG_PIOD_SODR = PIO_SODR_P1; //reset til høy, slik at den ikke resetter   
}

//Må man sjekke txrdy-flagget før man skrver til reg?
void motor_dac_set_speed(int16_t data){
    //uint8_t speed = data;
    if (data < 0){
        printf("venstre \n\r");
        REG_PIOD_SODR = PIO_SODR_P10; //går mot venstre
    }
    else{
        printf("høyre \n\r");
        REG_PIOD_CODR = PIO_CODR_P10; //går mot høyre
    }
    DACC->DACC_CDR = (data << 8);
}


uint32_t motor_turnon(){
    REG_PIOD_SODR = PIO_PD9;
    return 0;
} 

uint32_t motor_turnoff(){
    REG_PIOD_CODR = PIO_SODR_P9;
    return 0;
}

void delay(uint32_t ms){
    ms = ms * 139;
    //6962
    while(ms){
            ms--;
    }
}

void motor_reset_encoder(){
     //toggle !RST to reset encoder
     REG_PIOD_CODR |= PIO_SODR_P1;
     delay(1);
     REG_PIOD_SODR |= PIO_SODR_P1;
}

uint16_t motor_read_encoder(){

    uint16_t data = 0;

    REG_PIOD_CODR |= PIO_SODR_P0; //sets !OE, can read MJ2-pins
    PIOD->PIO_CODR = PIO_CODR_P0;
    REG_PIOD_CODR |= PIO_SODR_P2; //sel low

    //wait 20 microsek
    delay(1); 
    data |= (REG_PIOC_PDSR & 0x1FE) << 7; //data status register = data 

   REG_PIOD_SODR = PIO_SODR_P2; //Sel high

    delay(1);
    data |= (REG_PIOC_PDSR & 0x1FE) << 1;

    //toggle !RST to reset encoder
    //motor_reset_encoder();

    REG_PIOD_SODR |= PIO_SODR_P0; //set OE high
   

    return data;
}


