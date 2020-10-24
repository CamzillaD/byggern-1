#ifndef IR_H
#define IR_H

//driveren skal lese ir-signalet gjennom adc-en til arduinoen

uint32_t ir_adc_init();

uint32_t ir_read_signal();

#endif