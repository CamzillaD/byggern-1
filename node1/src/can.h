#ifndef CAN_H
#define CAN_H
#include <stdint.h>
#include <avr/interrupt.h>

typedef struct {
    uint16_t id;
    uint8_t size;
    uint8_t * buffer; //meldingen
} CanFrame;

void can_init();

void can_send(const CanFrame * p_frame);

uint8_t can_recv(CanFrame * p_frame);

uint8_t can_test();

#endif
