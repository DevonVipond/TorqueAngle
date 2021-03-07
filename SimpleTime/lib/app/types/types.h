
#pragma once

namespace app{
    typedef unsigned long timestamp;
    typedef unsigned int TorqueAngle;
    typedef unsigned int frequency;
    typedef unsigned int digital_measurement; // NOTE: this is a digital sample
    typedef unsigned int analog_measurement; // NOTE: this is a analog sample

    typedef bool CURRENT_MODE;
    static const CURRENT_MODE SYNC_CLOCKS_MODE  = false;
    static const CURRENT_MODE TORQUE_ANGLE_MODE = true;

    typedef short MESSAGE_TYPE;
    static const MESSAGE_TYPE ENTER_SYNC_CLOCKS_MODE_MSG  = 0;
    static const MESSAGE_TYPE ENTER_TORQUE_ANGLE_MODE_MSG = 1;
    static const MESSAGE_TYPE ROUND_TRIP_MSG              = 2;
    static const MESSAGE_TYPE SYNC_CLOCK_MSG              = 3;
    static const MESSAGE_TYPE SENSOR_TIMESTAMP_MSG        = 4;
};

