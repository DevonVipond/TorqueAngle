#pragma once

#include "../../app/types/types.h"

namespace infra {

    static const app::timestamp AVERAGE_EXECUTION_TIME_OF_MICROS = 0;

    app::timestamp get_current_time() {
        return micros() - AVERAGE_EXECUTION_TIME_OF_MICROS;
    }

}