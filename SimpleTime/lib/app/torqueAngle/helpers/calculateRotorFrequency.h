#pragma once

#include "../../types/types.h"
#include "calcuateTimeShift.h"

namespace app {
    unsigned int __calculate_rotor_frequency(const vector<timestamp>& peak_terminal_voltage_points) {

        unsigned int sum_period = 0;
        unsigned int num_data_points = peak_terminal_voltage_points.size();

        for (int i = 0; i < peak_terminal_voltage_points.size()-1; i++) {
            sum_period += __calculate_time_shift(peak_terminal_voltage_points[i+1], peak_terminal_voltage_points[i]);
        }

        auto sum_frequency = 1 / sum_period;
        auto frequency = sum_frequency / num_data_points;

        return frequency;
    }

}
