#include <vector>
#include <mutex>
#include <thread>
#include <memory>

#include "lib/app/types/types.h"
#include "lib/infra/protocols/espNow/ESPNowProtocol.h"
#include "lib/infra/sensor/readSenserData.h"
#include "lib/infra/terminalVoltage/readTerminalVoltage.h"
#include "lib/app/torqueAngle/calculateTorqueAngle.h"

infra::ESPNowReceiver<app::timestamp> receiver;

std::vector<app::timestamp> receiver_buffer;
std::vector<app::timestamp> terminal_voltage_buffer;

app::timestamp no_load_time_shift = 0;

std::unique_ptr<std::thread> terminal_voltage_thread;
std::mutex terminal_voltage_mutex;

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

    {
        std::unique_lock<std::mutex> lock(terminal_voltage_mutex);
        receiver_buffer.push_back(incomingMessage);

        update_torque_angle();
    }
}

void setupReciever()
{

    Serial.begin(115200);

    receiver.init(message_received_callback);

    terminal_voltage_thread.reset(new std::thread([&]() {
        while (1)
        {
            auto time = infra::wait_for_next_terminal_voltage_peak();
            {
                std::unique_lock<std::mutex> lock(terminal_voltage_mutex);

                terminal_voltage_buffer.push_back(time);

            }
        }
    }));
}
