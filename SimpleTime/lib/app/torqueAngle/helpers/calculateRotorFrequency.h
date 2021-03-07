#pragma once

#include <queue>

#include "../../types/types.h"
#include "calculateTimeShift.h"

namespace app {
    frequency __calculate_rotor_frequency(std::queue<timestamp>& peak_terminal_voltage_points) {
        // TODO: PROPERLY IMPLEMENT
        return 3600;

        //frequency min_period = std::numeric_limits<unsigned int>::max();

        //for (int i = 0; i < peak_terminal_voltage_points.size(); i++) {
        //    min_period = std::min(__calculate_time_shift(peak_terminal_voltage_points[i+1], peak_terminal_voltage_points[i]), min_period);
        //}


        //return 1 / min_period;
    }

    //unsigned int __calculate_rotor_frequency(const std::queue<timestamp>& peak_terminal_voltage_points) {

    //    unsigned int sum_period = 0;
    //    unsigned int num_data_points = peak_terminal_voltage_points.size();

    //    for (int i = 0; i < peak_terminal_voltage_points.size()-1; i++) {
    //        sum_period += __calculate_time_shift(peak_terminal_voltage_points[i+1], peak_terminal_voltage_points[i]);
    //    }

    //    auto sum_frequency = 1 / sum_period;
    //    auto frequency = sum_frequency / num_data_points;

    //    return frequency;
    //}


}
