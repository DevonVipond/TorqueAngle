#pragma once

#include "../../app/types/types.h"
#include "../../app/constants/constants.h"

namespace infra {

    timestamp get_current_time() {
        return micros() - app::AVERAGE_EXECUTION_TIME_OF_MICROS;
    }

}