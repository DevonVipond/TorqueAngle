#pragma once

#include "../../app/types/types.h"

namespace infra {

    static const timestamp AVERAGE_EXECUTION_TIME_OF_MICROS = 0;

    timestamp get_current_time() {
        return micros() - AVERAGE_EXECUTION_TIME_OF_MICROS;
    }

}