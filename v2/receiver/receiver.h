#pragma once
#include "state-machine.h"

namespace receiver
{
    void start()
    {;
        Clock clock;
        CommunicationChannel comms({ 0x3c,0x61,0x05,0x3e,0xee,0xa4 }, {0x3C, 0x61, 0x05, 0x3E, 0xD7, 0x4C});
        StateMachine stateMachine(clock, comms);

        stateMachine.run();
    }
}