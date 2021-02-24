#ifndef CALCTORQUEANGLE_H 
#define CALCTORQUEANGLE_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "lib/app/types/types.h"

using namespace types;

//timestamp get_current_time() { return micros(); };

timestamp calculate_time_shift(timestamp t1, timestamp t2) {

    timestamp time_diff = t1 - t2;

    return time_diff;
}

TorqueAngle calculate_torque_angle(timestamp reference_point,
                            timestamp terminal_voltage_peak,
                            timestamp no_load_time_diff,
                            RotorFrequency rotor_frequency ) {

    timestamp time_diff = calculate_time_shift(reference_point, terminal_voltage_peak);

    time_diff = calculate_time_shift(time_diff, no_load_time_diff);

    TorqueAngle torque_angle = 360 * time_diff * rotor_frequency;

    return torque_angle;
}




#endif