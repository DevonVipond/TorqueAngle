#pragma once

#include <stdio.h>
#include <cmath>

#include "../../types/types.h"

namespace app {
    timestamp __calculate_time_shift(timestamp t1, timestamp t2) {

        return std::abs(t1 - t2);
    }
}
