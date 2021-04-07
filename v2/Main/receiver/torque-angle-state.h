#pragma once

#include <vector>
#include "../shared/state.h"
#include "../shared/gpio-reader.h"
#include "torque-angle-calculator.h"

using namespace types;
using namespace shared;

namespace receiver
{
    static const unsigned int TERMINAL_VOLTAGE_PIN = 23;
    static const unsigned int ZERO_TORQUE_ANGLE_PIN = 22;

    class TorqueAngleState : public State
    {
    private:
        std::vector<timestamp> _referencePoints;
        std::vector<timestamp> _zeroCrossings;
        GPIOReader _terminalVoltageReader;
        GPIOReader _zeroTorqueAngleReader;
        TorqueAngleCalculator _calculator;

        bool shouldZeroTorqueAngle() { return (_zeroTorqueAngleReader.readPin() == 1); }
        bool shouldCalculateTorqueAngle() { return (_referencePoints.size() > 40 && _zeroCrossings.size() > 40); }

        void timestampNextRisingEdge()
        {
            _zeroCrossings.push_back(_terminalVoltageReader.timestampNextRisingEdge());
        }

        void zeroTorqueAngle()
        {
            try
            {
                _calculator.zeroTorqueAngle(std::move(_referencePoints), std::move(_zeroCrossings));
            }
            catch (std::exception)
            {
                log("Failed to zero torque angle :(");
            }
        }

        void calculateTorqueAngle()
        {
            try
            {
                auto torqueAngle = _calculator.calculate(std::move(_referencePoints), std::move(_zeroCrossings));
                log(torqueAngle);
            }
            catch (std::exception)
            {
                log("Failed to calculate torque angle :(");
            }
        }

        void setup()
        {
            State::_is_running = true;
        }
        void teardown()
        {
            _referencePoints.clear();
            _zeroCrossings.clear();
        }

    public:
        TorqueAngleState(Clock &clock) : _terminalVoltageReader(clock, TERMINAL_VOLTAGE_PIN),
                                                                      _zeroTorqueAngleReader(clock, ZERO_TORQUE_ANGLE_PIN),
                                                                      _calculator()
        {
        }
        TorqueAngleState() = delete;

        void handle_message(Message message)
        {
            using MT = Message::MESSAGE_TYPES;

            switch (message.type())
            {
            case MT::START_CLOCK_SYNC:
                stop();
                break;
            case MT::REFERENCE_POINT:
                _referencePoints.push_back(message.payload());
                break;
            default:
                log("clock-sync-state: Unknown message");
            };
        }

        void start()
        {
            setup();

            while (State::_is_running)
            {

                timestampNextRisingEdge();

                if (shouldZeroTorqueAngle())
                {
                    zeroTorqueAngle();
                }

                if (shouldCalculateTorqueAngle())
                {
                    calculateTorqueAngle();
                }
            }

            teardown();
        }
    };

}