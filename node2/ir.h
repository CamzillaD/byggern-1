#ifndef IR_H
#define IR_H

#include <stdint.h>

//driveren skal lese ir-signalet gjennom adc-en til arduinoen

uint32_t ir_adc_init();

uint8_t ir_beam_broken();




#endif