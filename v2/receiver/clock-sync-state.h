#pragma once

#include <vector>
#include <algorithm>
#include "../shared/communication-channel.h"
#include "../shared/logger.h"
#include "../shared/types.h"
#include "../shared/message.h"
#include "../shared/clock.h"

using namespace shared;
using namespace types;

namespace receiver
{

    class ClockSyncer
    {
    private:
        Clock &_clock;
        std::vector<timestamp> _clockOffsets;

    public:
        ClockSyncer(Clock &clock) : _clock(clock) {}

        addClockOffset(timestmap offset) { _clockOffsets.push_back(offset); }

        sync()
        {
            if (_clockOffsets.empty())
                return;

            std::sort(_clockOffsets.begin(), _clockOffsets.end());

            auto mid = _clockOffsets.size() / 2;

            _clock.setOffset(_clockOffsets[mid]);

            _clockOffsets.clear();
        }

    }

    class ClockSyncState : public State
    {
    private:
        const CommunicationChannel &_communicationChannel;
        ClockSyncer &_clockSyncer;

        void setup()
        {
            State::_is_running = true;
        }

        void teardown()
        {
        }

    public:
        ClockSyncState(const CommunicationChannel &communicationChannel, Clock &clock) : _communicationChannel(communicationChannel),
                                                                                         _clockSyncer(clock) {}
        ClockSyncState() = delete;

        void handle_message(Message message)
        {
            using MT = Message::MESSAGE_TYPES;

            switch (message.type())
            {
            case MT::START_TORQUE_ANGLE_CALCULATION:
                _clockSyncer.sync();
                stop();
                break;
            case MT::PING:
                _communicationChannel.sendMessage(message);
                break;
            case MT::SYNC_CLOCK_MESSAGE:
                _clockSycner.addClockOffset(static_cast<long int>(msg.payload()) - static_cast<long int>(micros()));
                break;
            default:
                log("Unknown message: ", message.type());
            };
        }

        void start()
        {
            setup();

            while (State::_is_running)
            {
                delayMicroseconds(10);
            }

            teardown();
        }
    };

}