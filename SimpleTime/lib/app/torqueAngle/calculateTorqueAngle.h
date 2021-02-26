#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "../types/types.h"
#include "helpers/calculateRotorFrequency.h"

using namespace std;

namespace app {

    TorqueAngle __calculate_torque_angle(const timestamp &reference_point,    const timestamp &terminal_voltage_peak,
                                         const timestamp &no_load_time_shift, const frequency &rotor_frequency) {

        timestamp time_shift = __calculate_time_shift(reference_point, terminal_voltage_peak);

        time_shift = __calculate_time_shift(time_shift, no_load_time_shift);

        TorqueAngle torque_angle = 360 * time_shift * rotor_frequency;

        return torque_angle;
    }

    TorqueAngle calculate_average_torque_angle(const vector <timestamp> &reference_points,
                                               const vector <timestamp> &peak_terminal_voltage_points,
                                               const timestamp &no_load_time_diff) {

        unsigned int rotor_frequency = __calculate_rotor_frequency(peak_terminal_voltage_points);
        auto max_iteration = std::min(reference_points.size(), peak_terminal_voltage_points.size());
        TorqueAngle sum_torque_angles = 0;

        for (unsigned int i = 0; i < max_iteration; i++) {
            timestamp reference = reference_points[i];
            timestamp peak_terminal_voltage = peak_terminal_voltage_points[i];

            sum_torque_angles = __calculate_torque_angle(reference, peak_terminal_voltage, no_load_time_diff, rotor_frequency);
        }

        auto torque_angle = sum_torque_angles / max_iteration;

        return torque_angle;
    }
}