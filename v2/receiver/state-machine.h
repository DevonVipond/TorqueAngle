#pragma once
#include <memory>
#include <atomic>

#include "./torque-angle-state.h"
#include "./clock-sync-state.h"
#include "../../shared/communication-channel.h"
#include "../../shared/logger.h"

using namespace types;
using namespace shared;

namespace receiver
{
    class StateMachine
    {

    private:
        State *_currentState;
        TorqueAngleState torqueAngleState(clock);
        ClockSyncState clockSyncState(communicationChannel);

    public:
        StateMachine(Clock &clock, CommunicationChannel communicationChannel) : _currentState(nullptr),
                                                                                torqueAngleState(clock),
                                                                                clockSyncState(communicationChannel, clock)
        {
        }

        void run()
        {

            auto messageReceivedCallback = [&](Message incomingMessage) {
                if (_currentStatePtr)
                    _currentStatePtr->handle_message(incomingMessage);
            };

            communicationChannel.subscribe(messageReceivedCallback);

            while (1)
            {
                log("Starting Clock Sync State!");
                _curentState = &clockSyncState;
                _currentState->start();

                log("Starting Torque Angle State!");
                _currentState = &torqueAngleState;
                _currentState->start();
            }
        }
    };
}