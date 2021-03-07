#pragma once

#include "../../app/types/types.h"
#include "../../app/globalVariables/globalVariables.h"
#include "../../app/constants/constants.h"

namespace infra {

    using namespace app;

    app::timestamp get_current_time() {

        long int curr_time = static_cast<long int> (micros()) + g_clock_offset;

        return static_cast<app::timestamp> (curr_time);
    }

}