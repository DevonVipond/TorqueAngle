#pragma once
#include <memory>
#include <atomic>

#include "./torque-angle-state.h"
#include "clock-sync-state.h"
#include "../shared/communication-channel.h"
#include "../shared/logger.h"
#include "../shared/observable.h"

using namespace types;
using namespace shared;

namespace receiver
{
    class StateMachine : public Observer
    {

    private:
        State *_currentState;
        TorqueAngleState torqueAngleState;
        ClockSyncState clockSyncState;

        void nextState()
        {
            if (_currentState == &clockSyncState)
            {
                log("Starting Torque Angle State!");
                _currentState = &torqueAngleState;
            }
            else
            {
                log("Starting Clock Sync State!");
                _currentState = &clockSyncState;
            }
        }

    public:
        StateMachine(Clock &clock, CommunicationChannel communicationChannel) : _currentState(nullptr),
                                                                                torqueAngleState(clock),
                                                                                clockSyncState(communicationChannel, clock)
        {
            communicationChannel.subscribe(this);
        }

        void observerCallback(Message incomingMessage)
        {
            if (_currentState)
                _currentState->handle_message(incomingMessage);
        }

        void run()
        {

            while (1)
            {
                nextState();
                _currentState->start();
            }
        }
    };
}