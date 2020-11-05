#include "solenoid.h"


void solenoid_init(){
    REG_PIOC_PER = PIO_PER_P12; //enable pin 51
    REG_PIOC_OER = PIO_PER_P12; //Output
}

void solenoid_activate(){
    REG_PIOC_SODR = PIO_SODR_P12;
}

void solenoid_deactivate(){
     REG_PIOC_CODR = PIO_CODR_P12;
}