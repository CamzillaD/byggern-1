#include "display.h"
#include <stdint.h>

#define DISPLAY_C_MEM ((volatile uint8_t *)0x1000)
#define DISPLAY_D_MEM ((volatile uint8_t *)0x1200)

void display_clear(){
    for(int j = 0; j < 8; j++){

        *DISPLAY_C_MEM = 0xb0 | j;
        *DISPLAY_C_MEM = 0x00;
        *DISPLAY_C_MEM = 0x1f;

        for (int i = 0; i < 128; i++){
        *DISPLAY_D_MEM = 0x00;
        }
    }
}

void display_push_ram_to_oled(){
    for (int i = 0; i < 8; i++){
        *DISPLAY_C_MEM = 0xB0 | i;  //set page ram start
        *DISPLAY_C_MEM = 0x00;
        *DISPLAY_C_MEM = 0x1f;
        if(i != 10){
            for(int j = 0; j < 128; j++){
                *DISPLAY_D_MEM = 0x01;
            
            }
        }
    }
}

void display_init(){
    *DISPLAY_C_MEM = 0xae;  //Display off

    *DISPLAY_C_MEM = 0xa0;  //Set column 0 as SEG0

    *DISPLAY_C_MEM = 0xda;  //Set COM Pins Hardware Configuration
    *DISPLAY_C_MEM = 0x12;  //Alternative COM pin configuration, Disable COM L/R remap

    *DISPLAY_C_MEM = 0xc8;  //Set COM output scan direction (COM0-COM[N-1])

    *DISPLAY_C_MEM = 0xa8;  //multiplex ration mode:63
    *DISPLAY_C_MEM = 0x3f;  //63

    *DISPLAY_C_MEM = 0xd5;  //Set Display Divide ratio
    *DISPLAY_C_MEM = 0x40;

    *DISPLAY_C_MEM = 0x81;  //Set contrast
    *DISPLAY_C_MEM = 0xaa;  //Value of contrast

    *DISPLAY_C_MEM = 0xd9;  //Set Pre-charge Period
    *DISPLAY_C_MEM = 0x21;

    *DISPLAY_C_MEM = 0x20;  //Set addressing mode to Page Addressing Mode
    *DISPLAY_C_MEM = 0x02;

    *DISPLAY_C_MEM = 0x00;  //Setting Column 0 as start address for page addressing mode

    *DISPLAY_C_MEM = 0xdb;  //Set V_COMH
    *DISPLAY_C_MEM = 0x30;

    *DISPLAY_C_MEM = 0xad;  //Set ext or int I_REF ?
    *DISPLAY_C_MEM = 0x00;

    *DISPLAY_C_MEM = 0x40;

    *DISPLAY_C_MEM = 0xa4;  //Set follow RAM

    *DISPLAY_C_MEM = 0xa6;  //Set normal display mode
    display_clear();
    
    *DISPLAY_C_MEM = 0xaf;  //turn display on
    //display_push_ram_to_oled();

    display_push_ram_to_oled();

    *DISPLAY_C_MEM = 0xB2;
    *DISPLAY_C_MEM = 0x00;
    *DISPLAY_C_MEM = 0x1f;
    for(int k = 0; k < 128; k++){
        *DISPLAY_D_MEM = 0x00;
    }

    
}

void display_reset(){
    *DISPLAY_C_MEM = 0xae;  //Display off

}


void display_home(){

}

void display_go_to_line(uint8_t line){

}

void display_go_to_column(uint8_t column){

}

void display_clear_line(uint8_t line){

}

void display_pos(uint8_t row, uint8_t column){

}

void display_print(uint8_t* chr){

}