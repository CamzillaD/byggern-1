#include "sram_driver.h"
#include <avr/io.h>

volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM


void ext_address_initialize(){
    
    MCUCR |= (1<<SRE);
    SFIOR |= (1<<XMM2); //mask out JTAG-pins from adress-line
}




void test_latch(){
while(1){
    ext_ram[0x0F]=0xFF;  //test av latch
    }
}
