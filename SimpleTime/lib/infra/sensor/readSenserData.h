#pragma once

#include "../../app/types/types.h"
#include "../../app/constants/constants.h"
#include "../time/getCurrentTime.h"

namespace infra {

    app::timestamp wait_for_rising_edge() {
        app::digital_measurement prev_measurement = 1;

        while (1) {

            app::digital_measurement measurement = digitalRead(app::SENSOR_PIN);

            bool is_rising_edge = measurement > prev_measurement;

            if (is_rising_edge) {
                return get_current_time();
            }

            prev_measurement = measurement;

            delayMicroseconds(app::ANALOG_SAMPLING_RATE_MICROSECONDS);
        }
    }
}