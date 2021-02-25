#pragma once

#include "../time/getCurrentTime.h"
#include "../../app/types/types.h"

namespace infra {

    template< typename Protocol >
    void sync_clocks(Protocol protocol) {
        timestamp current_time = get_current_time();

        protocol.send_message(0);
        protocol.send_message(current_time);
    }
}