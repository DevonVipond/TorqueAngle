#include "lib/infra/protocols/espNow/ESPNowProtocol.h"
#include "lib/infra/sensor/read_sensor_data.h"

typedef unsigned int PayloadType;
static const unsigned int DELAY_PERIOD = 0;

infra::ESPNowTransmitter< PayloadType > transmitter;

void setup() {
}

void loop() {
    auto rising_edge_timestamp = infra::wait_for_rising_edge();

    try {

        transmitter.send_message( rising_edge_timestamp );

    } catch (infra::ConnectionFailed &e) {

        Serial.printline(e.what())
        exit(0);
    }

    //delay(DELAY_PERIOD);
}

