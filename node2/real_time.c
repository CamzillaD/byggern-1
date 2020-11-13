#include "real_time.h"
#include "sam.h"

void real_time_init(){
    /* Reset Real time timer, 125 ms tick rate */
    RTT->RTT_MR = RTT_MR_RTTRST | 0x1000;
}

uint8_t real_time_incremented(){
    return RTT->RTT_SR & RTT_SR_RTTINC;
}