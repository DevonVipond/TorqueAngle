#include <vector>
#include "lib/app/terminalVoltageHelpers/getPeakTerminalVoltageTimestamps.h"
#include "lib/infra/protocols/espNow/ESPNowProtocol.h"
#include "lib/app/buffer/buffer.h"
#include "lib/app/types/types.h"

  infra::ESPNowReciever< PayloadType > receiver;

  std::vector<timestamp> receiver_buffer;
  std::vector<app::TerminalVoltagePoint> terminal_voltage_buffer;

  app::timestamp no_load_time_shift = 0;

  struct message {
      app::timestamp payload;
  };

  void display_torque_angle(TorqueAngle torque_angle) {
      Serial.println(torque_angle);
  }

  void update_torque_angle() {
      if (receiver_buffer.size() < 100) return;

      auto peak_terminal_voltage_timestamps = app::get_peak_terminal_voltage_timestamps(terminal_voltage_buffer);
      auto torque_angle = app::calculate_average_torque_angle(receiver_buffer, terminal_voltage_buffer, no_load_time_shift);

      terminal_voltage_buffer.clear()
      receiver_buffer.clear()

      display_torque_angle();
  }

void message_received_callback(const uint8_t * mac, const uint8_t *incomingData, int len) {
      message incomingMessage;
      memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

      receiver_buffer.push_back(incomingMessage.payload);

      update_torque_angle();
  }

  void setup() {

      Serial.begin(115200);

      receiver.init(message_recieved_callback);
  }


  void loop() {

      static unsigned int SAMPLING_PERIOD_MILLISECONDS = 2;
      terminal_voltage_buffer.push_back(infra::read_terminal_voltage());

      delay(SAMPLING_PERIOD_MILLISECONDS);
  }

