#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iterator>

#include "../types/types.h"
#include "helpers/calculateRotorFrequency.h"
#include "calculationFailedException.h"
#include "../../infra/logger/logger.h"

using namespace std;

namespace app {

    #define MAX_TORQUE_ANGLE 90.0
    #define MAX_TIME_SHIFT_MICROSECONDS 5000 
    #define MICROSECONDS_IN_MILLISECOND 1000.0

    double convert_microsecond_to_millisecond(const app::timestamp& microsecond_timestmap) {

        return static_cast<double>(microsecond_timestmap) / MICROSECONDS_IN_MILLISECOND;

    }

    void dump_contents(const timestamp &reference_point, const std::vector < timestamp > &zero_crossings) {

        Serial.println("Unable to find terminal voltage zero crossing ");
        Serial.print("reference point: ");
        Serial.print(reference_point);
        Serial.print(" zero crossings: ");
        int counter = 0;
        for (auto zero : zero_crossings) {
            Serial.print(zero);

            if (counter == 0) {
                Serial.println(",");
            } else {
                Serial.print(" , ");

            }

            counter = (counter + 1) % 8;
        }

    }


    TorqueAngle __calculate_torque_angle(const timestamp &reference_point, const std::vector < timestamp > &zero_crossings, const timestamp &no_load_time_shift) {

        timestamp min_time_shift = 5000;
        for (timestamp crossing : zero_crossings) {

             min_time_shift = std::min(min_time_shift, __calculate_time_shift(reference_point, crossing));

        }

        //Serial.print("calculated time shift: ");
        //Serial.println(min_time_shift);

        if (min_time_shift >= 5000) {
            //dump_contents(reference_point, zero_crossings);
            throw app::CalculationFailed();
        }

        double time_shift_milliseconds = convert_microsecond_to_millisecond(min_time_shift);

        TorqueAngle torque_angle = (90.0 / 4.0) * ( static_cast<TorqueAngle>(time_shift_milliseconds) - static_cast<TorqueAngle>(no_load_time_shift) ); //* static_cast<double>(rotor_frequency);

        return torque_angle;
    }


}