#pragma once

#include "../../app/types/types.h"
#include "../../app/constants/constants.h"
#include "../time/getCurrentTime.h"

namespace infra {

    timestamp wait_for_next_terminal_voltage_peak() {
        bool prev_rising = false;
        app::analog_measurement prev_measurement = 0;

        while (1) {
            app::analog_measurement measurement = analogRead(app::TERMINAL_VOLTAGE_PIN);

            bool falling = measurement < prev_measurement;
            if (prev_rising && falling) {
                return get_current_time();
            }

            prev_measurement = measurement;
            prev_rising = measurement > prev_measurement;

            delayMicroseconds(app::ANALOG_SAMPLING_RATE_MICROSECONDS);
        }
    }
}
