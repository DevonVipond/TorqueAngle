#pragma once

#include <vector>
#include "../types/types.h"

using namespace std;

namespace app{

    vector<timestamp> get_peak_terminal_voltage_timestamps(vector<TerminalVoltagePoint> terminal_voltage_points) {

        const is_increasing = [](unsigned int first_measurement, unsigned int second_measurement) -> bool {
            return second_meeasurement > first_measurement;
        };

        vector<timestamp> peak_timestamps;

        bool previously_increasing = is_increasing(terminal_voltage_points[0], terminal_voltage_points[1]);

        for (size_t i = 0; i < terminal_voltage_points.size();  i++) {

            unsigned int first_measurement = terminal_voltage_points[i].get_measurement();
            unsigned int second_measurement = terminal_voltage_points[i+1].get_measurement();
            timestamp time = terminal_voltage_points[i].get_timestamp();

            bool increasing = is_increasing(first_measurement, second_measurement);

            // check for peak
            if (!increasing && previously_increasing) {
                peak_timestamps.push_back(time);
            }

            previously_increasing = increasing;
        }

        return peak_timestamps;
    }
}
