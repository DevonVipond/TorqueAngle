#pragma once

#include "logger.h"
#include "types.h"

using namespace types;

namespace shared
{
    class Clock
    {

    private:
        long int _clockOffset;

    public:
        Clock() : _clockOffset(0)
        {
            log("Constructing Clock!");
        }
        void setOffset(timestamp offset)
        {
            _clockOffset = offset;
        }

        timestamp currentTime() const
        {
            long int time = static_cast<long int>(micros()) + _clockOffset;
            return static_cast<timestamp>(time);
        }
    };

}