#pragma once

#include <vector>
#include <limits.h>
#include <algorithm>
#include "../shared/logger.h"
#include "../shared/types.h"
#include "../shared/gpio-reader.h"
#include "../shared/communication-channel.h"
#include "../shared/message.h"
#include "../shared/observable.h"

using namespace types;
using namespace shared;

namespace transmitter
{

    static const unsigned int PHOTOELECTRIC_SENSOR_INPUT = 20;

    class TorqueAngleState : public State
    {
    private:
        static const unsigned int STATE_LIFETIME_MICROSECONDS = 1000 * 1000 * 60 * 2;
        Clock &_clock;
        CommunicationChannel &_comms;

        bool shouldLeaveState(timestamp startTime)
        {
            if (_clock.currentTime() - startTime > STATE_LIFETIME_MICROSECONDS * 1000 * 1000 * 60 * 2)
            {
                return true;
            }
            return false;
        }

        void transmitReferencePoint(timestamp referencePoint)
        {
            _comms.sendMessage(Message(Message::MESSAGE_TYPES::REFERENCE_POINT, referencePoint));
        }

    public:
        TorqueAngleState(CommunicationChannel comms, Clock &clock) : _clock(clock),
                                                                     _comms(comms)
        {
        }

        void handle_message(Message message)
        {
            log("Received unkown message: ");
        }

        void stop()
        {
            State::_is_running = false;
        }

        void start()
        {
            auto number_bolts = 4; // TODO: REMOVE
            auto bolts_counter = 0;

            GPIOReader sensor(_clock, PHOTOELECTRIC_SENSOR_INPUT);

            auto startTime = _clock.currentTime();

            while (State::_is_running)
            {
                try
                {

                    auto rising_edge_timestamp = sensor.timestampNextRisingEdge();

                    bolts_counter = (bolts_counter + 1) % number_bolts;

                    if (bolts_counter == 0)
                    {
                        log("sending rising edge timestamp");
                        transmitReferencePoint(rising_edge_timestamp);
                    }

                    if (shouldLeaveState(startTime))
                    {
                        stop();
                    }
                }
                catch (std::exception e)
                {
                    log("exception in torque-angle-state: ", e.what());
                }
            }
        };
    };
}