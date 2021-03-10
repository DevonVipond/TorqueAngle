#pragma once

#include <stdio.h>
#include <cmath>

#include "../../types/types.h"

namespace app {
    timestamp __calculate_time_shift(timestamp t1, timestamp t2) {

        return static_cast<timestamp>(std::abs(static_cast<double>(t1) - static_cast<double>(t2)));
    }
}
