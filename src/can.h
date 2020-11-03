#ifndef CAN_H
#define CAN_H
#include <stdint.h>

typedef struct {
    uint16_t id;
    uint8_t size;
    uint8_t buffer[8];
} CanFrame;

void can_init();

uint8_t can_send(const CanFrame * p_frame);

uint8_t can_recv(CanFrame * p_frame);

#endif
