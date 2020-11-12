#include "real_time.h"
#include "sam.h"

void real_time_delay_ms(uint32_t ms){
    /* Period 1/10 th of a millisecond */
    RTT->RTT_MR = RTT_MR_RTTRST | RTT_MR_RTPRES(3);

    while(RTT->RTT_VR < ms * 10);
}

