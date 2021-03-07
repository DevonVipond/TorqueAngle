
#include <queue>
#include <memory>

#include "lib/app/types/types.h"
#include "lib/app/globalVariables/globalVariables.h"
#include "lib/app/message/message.h"
#include "lib/infra/protocols/espNow/espNowReciever.h"
#include "lib/infra/protocols/espNow/espNowTransmitter.h"
#include "lib/infra/sensor/readSenserData.h"
#include "lib/infra/terminalVoltage/readTerminalVoltage.h"
#include "lib/app/torqueAngle/calculateTorqueAngle.h"
#include "lib/infra/syncClocks/syncClocks.h"

namespace reciever {
infra::ESPNowReceiver<app::message> receiver;
infra::ESPNowTransmitter< app::message > transmitter;

std::queue<app::timestamp> receiver_buffer;
std::queue<app::timestamp> terminal_voltage_zero_crossings_buffer;

app::CURRENT_MODE mode = app::SYNC_CLOCKS_MODE;

app::timestamp no_load_time_shift = 0;

using namespace app;
using namespace infra;

void display_torque_angle(app::TorqueAngle torque_angle) {

    Serial.print("torque angle: ");
    Serial.println(torque_angle);
}

void update_torque_angle() {

    auto torque_angle = app::calculate_average_torque_angle(receiver_buffer, terminal_voltage_buffer, no_load_time_shift);

    while (terminal_voltage_zero_crossings_buffer.size()) terminal_voltage_zero_crossings_buffer.pop();
    while (receiver_buffer.size()) receiver_buffer.pop();

    display_torque_angle(torque_angle);
}

void zero_torque_angle(app::timestamp no_load_time_shift) {

    Globals::instance()->g_no_load_time_shift = no_load_time_shift;

}

void set_clock_offset(const app::timestamp &transmitter_clock) {

    Globals::instance()->g_clock_offset = static_cast<long int>(transmitter_clock) - static_cast<long int>(get_current_time());

}

std::string msg_type_to_string(app::MESSAGE_TYPE type) {
    switch (type) {
        case: app::ENTER_SYNC_CLOCKS_MODE_MSG:
            return "ENTER_SYNC_CLOCKS_MODE_MSG";
        case: app::ENTER_TORQUE_ANGLE_MODE_MSG 
            return "ENTER_TORQUE_ANGLE_MODE_MSG";
        case: app::ROUND_TRIP_MSG             
            return "ROUND_TRIP_MSG";
        case: app::SYNC_CLOCK_MSG             
            return "SYNC_CLOCK_MSG";
        case: app::SENSOR_TIMESTAMP_MSG
            return "SENSOR_TIMESTAMP_MSG";
    }

    return "undefined type!";
}

void message_handler(app::message & msg) {
    Serial.print("received msg:");
    Serial.print(msg.payload);
    Serial.println(msg_type_to_string(msg.message_type));

    switch (msg.message_type) {
        case app::ENTER_SYNC_CLOCKS_MODE_MSG:

            mode = app::SYNC_CLOCKS_MODE;
            while (terminal_voltage_zero_crossings_buffer.size()) terminal_voltage_zero_crossings_buffer.pop();
            while (receiver_buffer.size()) receiver_buffer.pop();
            break;
        case app::ENTER_TORQUE_ANGLE_MODE_MSG:

            mode = app::TORQUE_ANGLE_MODE;
            while (terminal_voltage_zero_crossings_buffer.size()) terminal_voltage_zero_crossings_buffer.pop();
            while (receiver_buffer.size()) receiver_buffer.pop();
            break;
        case app::ROUND_TRIP_MSG:

            if (mode != app::SYNC_CLOCKS_MODE) {
                throw std::exception();
            }

            transmitter.send_message( round_trip_message(msg.payload) );
            break;
        case app::SYNC_CLOCK_MSG:

            if (mode != app::SYNC_CLOCKS_MODE) {
                throw std::exception();
            }

            set_clock_offset(msg.payload);

            break;
        case app::SENSOR_TIMESTAMP_MSG:

            if (mode != app::TORQUE_ANGLE_MODE) {
                throw std::exception();
            }

            receiver_buffer.push(msg.payload);
            break;
        default:
            throw std::exception();
    };

}

void message_received_callback(const uint8_t *mac, const uint8_t *incomingData, int len) {

    app::message incoming_message;
    memcpy(&incoming_message, incomingData, sizeof(incoming_message));

    message_handler(incoming_message);
}

void setupReciever() {

    g_clock_offset = 0;

    Serial.begin(115200);

    receiver.init(message_received_callback);

}

void loopReciever() {

    Serial.println(mode == app::TORQUE_ANGLE_MODE ? "TORQUE_ANGLE_MODE" : "SYNC_CLOCKS_MODE");

    if (mode == app::TORQUE_ANGLE_MODE) {

        auto zero_crossing_timestamp = infra::wait_for_rising_edge(app::TERMINAL_VOLTAGE_PIN);

        terminal_voltage_zero_crossings_buffer.push(zero_crossing_timestamp);

        if (receiver_buffer.size() >= 100)
            update_torque_angle();
    }

}

};