#pragma once
#include "state-machine.h"

namespace receiver
{
    void start()
    {;
        Clock clock;
        uint8_t transmitterAddress[] = {0x3C, 0x61, 0x05, 0x3E, 0xD7, 0x4C};
        uint8_t receiverAddress[] = { 0x3c,0x61,0x05,0x3e,0xee,0xa4 };
        CommunicationChannel comms(receiverAddress, transmitterAddress);
        StateMachine stateMachine(clock, comms);

        stateMachine.run();
    }
}