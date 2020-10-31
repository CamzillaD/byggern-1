#ifndef NETWORK_H
#define NETWORK_H
#include <stdint.h>

typedef enum {
    NETWORK_STATE_CONNECTED,
    NETWORK_STATE_DISCONNECTED
} NetworkState;

void network_init();

uint8_t network_message_read(NetworkMessage * p_message);

void network_indicate(NetworkState state);

#endif
