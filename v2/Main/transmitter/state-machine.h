#pragma once
#include <memory>
#include <atomic>

#include "../shared/communication-channel.h"
#include "../shared/logger.h"
#include "torque-angle-state.h"
#include "clock-sync-state.h"

using namespace types;
using namespace shared;

namespace transmitter
{
    class StateMachine
    {

    private:
        TorqueAngleState torqueAngleState;
        ClockSyncState clockSyncState;

    public:
        StateMachine(Clock &clock, CommunicationChannel communicationChannel) : torqueAngleState(communicationChannel, clock),
                                                                                clockSyncState(clock, communicationChannel)
        {
        }

        void run()
        {
            while (1)
            {
                log("Starting Clock Sync State!");
                clockSyncState.start();

                log("Starting Torque Angle State!");
                torqueAngleState.start();
            }
        }
    };
}