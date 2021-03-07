
#include <queue>

#include "lib/app/types/types.h"
#include "lib/app/message/message.h"
#include "lib/infra/protocols/espNow/espNowTransmitter.h"
#include "lib/infra/protocols/espNow/espNowReciever.h"
#include "lib/infra/sensor/readSenserData.h"

namespace transmiter {

infra::ESPNowTransmitter< app::timestamp > transmitter;
infra::ESPNowReceiver< app::timestamp > reciever;

std::queue<app::timestamp> reciever_buffer;

app::CURRENT_MODE mode = app::SYNC_CLOCKS;

void message_received_callback(const uint8_t *mac, const uint8_t *incomingData, int len) {

    app::message incomingMessage;
    memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

    reciever_buffer.push(incomingMessage.payload);
}

void setupTransmitter() {

    reciever.init(message_received_callback);
}

void loopTransmitter() {
    if (mode == app::SYNC_CLOCKS_MODE) {

        infra::SyncClocks<
                            infra::ESPNowReceiver< app::message >,
                            infra::ESPNowTransmitter< app::message >
        > sync_clocks(reciever, transmitter, reciever_buffer);

        sync_clocks.start();

        mode = app::TORQUE_ANGLE_MODE;

    } else if (mode == app::TORQUE_ANGLE_MODE) {

        auto rising_edge_timestamp = infra::wait_for_rising_edge(app::SENSOR_PIN);

        transmitter.send_message( sensor_timestamp_message(rising_edge_timestamp) );
    }
}

};