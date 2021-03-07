#include <queue>
#include <memory>

#include "lib/app/types/types.h"
#include "lib/infra/protocols/espNow/espNowReciever.h"
#include "lib/infra/sensor/readSenserData.h"
#include "lib/infra/terminalVoltage/readTerminalVoltage.h"
#include "lib/app/torqueAngle/calculateTorqueAngle.h"
#include "lib/infra/syncClocks/syncClocks.h"

infra::ESPNowReceiver<app::timestamp> receiver;
infra::ESPNowTransmitter< app::timestamp > transmitter;

std::queue<app::timestamp> receiver_buffer;
std::queue<app::timestamp> terminal_voltage_buffer;

app::timestamp no_load_time_shift = 0;


struct message
{
    app::timestamp payload;
} message;

//void display_torque_angle(app::TorqueAngle torque_angle)
//{
//    Serial.println(torque_angle);
//}

void update_torque_angle()
{
    if (receiver_buffer.size() < 100)
        return;

    auto torque_angle = app::calculate_average_torque_angle(receiver_buffer, terminal_voltage_buffer, no_load_time_shift);

    terminal_voltage_buffer.clear();
    receiver_buffer.clear();

    //display_torque_angle(torque_angle);
}

void message_received_callback(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    app::timestamp incomingMessage;
    memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

    receiver_buffer.push_back(incomingMessage);

    update_torque_angle();
}

void setupReciever()
{

    Serial.begin(115200);

    receiver.init(message_received_callback);

}

void loopReciever() {
    if (mode = MODE::SYNC_CLOCKS) {
        if (!receiver_buffer.size()) continue;

        app::timestamp last_recieved_message = receiver_buffer.pop();

        if (last_recieved_message == SyncClocks::FINISH_CLOCK_SYNC_MODE_MESSAGE) {
            mode = MODE::NORMAL_OPERATION;
            continue;
        }

        transmitter.send_message(last_recieved_message);

    } else {

        auto time = infra::wait_for_next_terminal_voltage_peak();

        terminal_voltage_buffer.push_back(time);
    }

}
