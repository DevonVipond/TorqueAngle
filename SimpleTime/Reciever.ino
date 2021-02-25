#include <vector>
#include <memory>

#include "lib/app/terminalVoltageHelpers/getPeakTerminalVoltageTimestamps.h"
#include "lib/infra/protocols/espNow/ESPNowProtocol.h"
#include "lib/infra/sensor/readSensorData.h"
#include "lib/app/buffer/buffer.h"
#include "lib/app/types/types.h"

  infra::ESPNowReceiver< PayloadType > receiver;

  std::vector<timestamp> receiver_buffer;
  std::vector<app::TerminalVoltagePoint> terminal_voltage_buffer;

  app::timestamp no_load_time_shift = 0;

  std:unique_ptr<std::thread> terminal_voltage_thread;
  std::mutex terminal_voltage_mutex;

struct message {
      app::timestamp payload;
  };

  void display_torque_angle(TorqueAngle torque_angle) {
      Serial.println(torque_angle);
  }

  void update_torque_angle() {
      if (receiver_buffer.size() < 100) return;

      auto torque_angle = app::calculate_average_torque_angle(receiver_buffer, terminal_voltage_buffer, no_load_time_shift);

      terminal_voltage_buffer.clear()
      receiver_buffer.clear()

      display_torque_angle();
  }

void message_received_callback(const uint8_t * mac, const uint8_t *incomingData, int len) {
      message incomingMessage;
      memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

      {
          std::unique_lock<std::mutext> lock (terminal_voltage_mutex)
          receiver_buffer.push_back(incomingMessage.payload);

          update_torque_angle();
      }
  }

  void setup() {

      Serial.begin(115200);

      receiver.init(message_recieved_callback);

      terminal_voltage_thread.reset(new std::thread([&]() {
          while (1) {
              auto time = infra::wait_for_next_terminal_voltage_peak();

              std::unique_lock<std::mutex> lock(terminal_voltage_mutex)

              terminal_voltage_buffer.push_back(time);

              lock.release();
          }
      }))
  }


  void loop() { }

