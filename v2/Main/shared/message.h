#pragma once

#include "../shared/types.h"
using namespace types;

namespace shared
{
    class Message
    {
    public:
        Message(char type, timestamp payload): _type(type), _payload(payload) {}
        Message() {}
        enum MESSAGE_TYPES
        {
            START_CLOCK_SYNC,
            START_TORQUE_ANGLE_CALCULATION,
            PING,
            SYNC_CLOCK_MESSAGE,
            REFERENCE_POINT,
        };

        char type () const { return _type; }
        timestamp payload() const { return _payload; }

    private:
        char _type;
        long int _payload;
    };

}