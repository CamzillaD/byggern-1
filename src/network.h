#ifndef NETWORK_H
#define NETWORK_H

typedef enum {
    NETWORK_STATE_CONNECTED,
    NETWORK_STATE_DISCONNECTED
} NetworkState;

void network_init();

void network_indicate(NetworkState state);

#endif
