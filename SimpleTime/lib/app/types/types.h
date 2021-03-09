
#pragma once

namespace app{
    typedef unsigned long timestamp;
    typedef double TorqueAngle;
    typedef unsigned int frequency;
    typedef unsigned int digital_measurement; // NOTE: this is a digital sample
    typedef unsigned int analog_measurement; // NOTE: this is a analog sample

    typedef char CURRENT_MODE;
    static const CURRENT_MODE SYNC_CLOCKS_MODE  = 'e';
    static const CURRENT_MODE TORQUE_ANGLE_MODE = 'f';

    typedef char MESSAGE_TYPE;
    static const MESSAGE_TYPE ENTER_SYNC_CLOCKS_MODE_MSG  = 'a';
    static const MESSAGE_TYPE ENTER_TORQUE_ANGLE_MODE_MSG = 'b';
    static const MESSAGE_TYPE ROUND_TRIP_MSG              = 'c';
    static const MESSAGE_TYPE SYNC_CLOCK_MSG              = 'd';
    static const MESSAGE_TYPE SENSOR_TIMESTAMP_MSG        = 'e';
};

