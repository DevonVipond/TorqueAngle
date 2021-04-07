#pragma once

#include "logger.h"
#include "types.h"
#include "clock.h"

namespace shared
{

    using namespace types;

    class GPIOReader
    {
    private:
        unsigned int _pin;
        const Clock &_clock;

    public:
        GPIOReader(const Clock &clock, const unsigned int &pin) : _pin(pin),
                                                                  _clock(clock)
        {
            pinMode(_pin, INPUT);
        }

        digitalMeasurement readPin()
        {
            return digitalRead(_pin);
        }

        timestamp timestampNextRisingEdge()
        {
            digitalMeasurement prev_measurement = 1;

            while (1)
            {

                digitalMeasurement measurement = readPin();

                bool is_rising_edge = measurement > prev_measurement;

                if (is_rising_edge)
                {
                    return _clock.currentTime();
                }

                prev_measurement = measurement;

                delayMicroseconds(10);
            }
        }
    };

}