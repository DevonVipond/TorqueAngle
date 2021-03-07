#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "../types/types.h"
#include "helpers/calculateRotorFrequency.h"

using namespace std;

namespace app {
    static const MAX_TORQUE_ANGLE = 90;

    TorqueAngle __calculate_torque_angle(const timestamp &reference_point,    queue < timestamp > &terminal_voltage_zero_crossings,
                                         const timestamp &no_load_time_shift, const frequency &rotor_frequency) {
        
        timestamp time_shift;
        bool found_terminal_voltage_zero_crossing = false;
        while (terminal_voltage_zero_crossings.size()) {
            app::timestamp zero_crossing = terminal_voltage_zero_crossings.front(); terminal_voltage_zero_crossings.pop();

            time_shift = __calculate_time_shift(reference_point, terminal_voltage_peak);

            time_shift = __calculate_time_shift(time_shift, no_load_time_shift);

            if (time_shift < MAX_TORQUE_ANGLE) {
                found_terminal_voltage_zero_crossing = true;
                break;
            }

        }

        if (!found_terminal_voltage_zero_crossing) return NULL;

        TorqueAngle torque_angle = 360 * time_shift * rotor_frequency;

        return torque_angle;
    }

    TorqueAngle calculate_average_torque_angle( queue <timestamp> &reference_points,
                                                queue <timestamp> &terminal_voltage_zero_crossings,
                                                const timestamp &no_load_time_diff) {

        frequency rotor_frequency = __calculate_rotor_frequency(terminal_voltage_zero_crossings);
        TorqueAngle sum_torque_angles = 0;

        auto max_iteration = std::min(reference_points.size(), terminal_voltage_zero_crossings.size());
        auto num_points = 0;

        for (unsigned int i = 0; i < max_iteration; i++) {
            timestamp reference = reference_points.front(); reference_points.pop();
            timestamp zero_crossing = terminal_voltage_zero_crossings.front(); reference_points.pop();

            auto torque_angle += __calculate_torque_angle(reference, zero_crossing, no_load_time_diff, rotor_frequency);

            if (torque_angle) {
                sum_torque_angles += torque_angle;
                num_points++;
            }
        }

        while (reference_points.size()) reference_points.pop();
        while (terminal_voltage_zero_crossings.size()) terminal_voltage_zero_crossings.pop();

        if (num_points == 0) return NULL;

        auto torque_angle = sum_torque_angles / num_points;

        return torque_angle;
    }
}