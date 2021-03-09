#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "../types/types.h"
#include "helpers/calculateRotorFrequency.h"
#include "calculationFailedException.h"
#include "../../infra/logger/logger.h"

using namespace std;

namespace app {
    #define MAX_TORQUE_ANGLE = 90;
    #define MAX_TIME_SHIFT_MICROSECONDS 4000
    #define MICROSECONDS_IN_MILLISECOND 1000

    app::timestamp convert_microsecond_to_millisecond(const app::timestamp& microsecond_timestmap) {

        return microsecond_timestmap / MICROSECONDS_IN_MILLISECOND;

    }


    TorqueAngle __calculate_torque_angle(const timestamp &reference_point,    queue < timestamp > &terminal_voltage_zero_crossings,
                                         const timestamp &no_load_time_shift, const frequency &rotor_frequency) {
        
        timestamp time_shift;
        bool found_terminal_voltage_zero_crossing = false;
        auto terminal_voltage_buffer_size = terminal_voltage_zero_crossings.size();

        for (size_t i=0; i < terminal_voltage_buffer_size; i++) {
            app::timestamp zero_crossing = terminal_voltage_zero_crossings.front(); terminal_voltage_zero_crossings.pop();

            time_shift = __calculate_time_shift(reference_point, zero_crossing);

            time_shift = __calculate_time_shift(time_shift, no_load_time_shift);

            if (time_shift < MAX_TIME_SHIFT_MICROSECONDS) {
                found_terminal_voltage_zero_crossing = true;
                break;
            }

        }

        if (!found_terminal_voltage_zero_crossing) {
            infra::log("Unable to find terminal voltage zero crossing");
            throw app::CalculationFailed();
        } 

        auto time_shift_milliseconds = convert_microsecond_to_millisecond(time_shift);

        TorqueAngle torque_angle = (90.0 / 4.0) * static_cast<double>(time_shift_milliseconds); //* static_cast<double>(rotor_frequency);

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

            auto torque_angle = __calculate_torque_angle(reference, terminal_voltage_zero_crossings, no_load_time_diff, rotor_frequency);

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

    //timestamp find_nearest_zero_crossing(const timestamp& reference_point, queue < timestamp > &terminal_voltage_zero_crossings) {
    //    while (terminal_voltage_zero_crossings.size()) {
    //        auto zero_crossing = terminal_voltage_zero_crossings.front(); terminal_voltage_zero_crossings.pop();

    //        auto time_shift = __calculate_time_shift(reference_point, terminal_voltage_zero_crossings);

    //        if (time_shift <= MAX_TIME_SHIFT_MICROSECONDS) {

    //            return zero_crossing;

    //        }

    //    }

    //    throw app::CalculationFailed;

    //}
}