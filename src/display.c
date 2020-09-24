#include "display.h"
#include <stdint.h>
#include <avr/interrupt.h>

#define DISPLAY_C_MEM ((volatile uint8_t *)0x1000)
#define DISPLAY_D_MEM ((volatile uint8_t *)0x1200)

static void display_seek_page(uint8_t page){
    *DISPLAY_C_MEM = (0xb0 | page);

    /* Start at column 0 */
    *DISPLAY_C_MEM = 0x00;
    *DISPLAY_C_MEM = 0x10;
}

void display_clear(){
    for(uint8_t page = 0; page < 8; page++){
        display_seek_page(page);
        for(uint8_t seg = 0; seg < 128; seg++){
            *DISPLAY_D_MEM = 0x00;
        }
    }
}

void display_init(){
    /* Globally disable interrupts during init */
    cli();

    /* Display off */
    *DISPLAY_C_MEM = 0xae;

    /* Segments remap */
    *DISPLAY_C_MEM = 0xa1;

    /* COM lines according to display wire up */
    *DISPLAY_C_MEM = 0xda;
    *DISPLAY_C_MEM = 0x12;

    /* COM lines scan direction 63..0 */
    *DISPLAY_C_MEM = 0xc8;

    /* Multiplex ratio 63 */
    *DISPLAY_C_MEM = 0xa8;
    *DISPLAY_C_MEM = 0x3f;

    /* Display clock divider */
    *DISPLAY_C_MEM = 0xd5;
    *DISPLAY_C_MEM = 0x80;

    /* Contrast */
    *DISPLAY_C_MEM = 0x81;
    *DISPLAY_C_MEM = 0x50; // aa

    /* Pre-charge period */
    *DISPLAY_C_MEM = 0xd9;
    *DISPLAY_C_MEM = 0x21;


    /* Page addressing mode */
    *DISPLAY_C_MEM = 0x20;
    *DISPLAY_C_MEM = 0x02;


    /* Voltage output */
    *DISPLAY_C_MEM = 0xdb;
    *DISPLAY_C_MEM = 0x30;

    /* External 510k current sense */
    *DISPLAY_C_MEM = 0xad;
    *DISPLAY_C_MEM = 0x00;

    /* OLED follows GDDSRAM */
    *DISPLAY_C_MEM = 0xa4;

    /* Display start line 0 */
    //*DISPLAY_C_MEM = 0x40;

    /* Normal (non-inverted) display */
    *DISPLAY_C_MEM = 0xa6;

    display_clear();

    /* Turn display on */
    *DISPLAY_C_MEM = 0xaf;

    /* Enable interrupts after init */
    sei();
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
