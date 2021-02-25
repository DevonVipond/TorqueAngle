#pragma once

#include "../../app/types/types.h"

namespace infra {

    app::TerminalVoltagePoint read_peak_terminal_voltage() {
        // measurement = analogRead(port);
        // timestamp = get_current_time();
        // return TerminalVoltagePoint(timestamp, measurement);
        return app::TerminalVoltagePoint(123, 456);
    }
}
