#include "play_ping_pong.h"
#include "sam.h"

uint32_t play_ping_pong_init(){
     RTT->RTT_MR = RTT_MR_RTTRST | RTT_MR_RTPRES(0x2000); //resets counter, counts by the second
}


uint32_t play_ping_pong_read_score(){
        return RTT->RTT_VR;  //leser hva som er på telleregisteret
}




