#ifndef NETWORK_H
#define NETWORK_H
#include <stdint.h>

typedef struct {
    uint8_t key;
    uint8_t value;
} NetworkMessage;

uint8_t network_message_read(NetworkMessage * p_message);

#endif
