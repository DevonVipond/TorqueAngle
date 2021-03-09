#pragma once

#include "../types/types.h"

namespace app {

    struct message {

        app::timestamp payload;
        app::MESSAGE_TYPE message_type;
        
    } ;

    message __message(app::timestamp payload, app::CURRENT_MODE mode) {

        message m;

        m.payload = payload;
        m.message_type = mode;

        return m;
    }

    message sync_clock_message (app::timestamp new_clock_time) {

        return __message(new_clock_time, app::SYNC_CLOCK_MSG);
    }

    message round_trip_message (app::timestamp current_time) {

        return __message(current_time, app::ROUND_TRIP_MSG);
    }

    message enter_sync_clocks_mode_message() {

        return __message(1, app::ENTER_SYNC_CLOCKS_MODE_MSG);
    }

    message enter_torque_angle_mode_message() {

        return __message(1, app::ENTER_TORQUE_ANGLE_MODE_MSG);
    }

    message sensor_timestamp_message(app::timestamp rising_edge_timestamp) {

        return __message(rising_edge_timestamp, app::SENSOR_TIMESTAMP_MSG);
    }


}
