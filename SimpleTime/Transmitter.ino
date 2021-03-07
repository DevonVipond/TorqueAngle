
#include <queue>

#include "lib/app/types/types.h"
#include "lib/infra/protocols/espNow/espNowTransmitter.h"
#include "lib/infra/protocols/espNow/espNowReciever.h"
#include "lib/infra/sensor/readSenserData.h"

namespace transmiter {
infra::ESPNowTransmitter< app::timestamp > transmitter;
infra::ESPNowReceiver< app::timestamp > reciever;

std::queue<app::timestamp> reciever_buffer;


app::MODE mode = app::MODE::SYNC_CLOCKS;

void message_received_callback(const uint8_t *mac, const uint8_t *incomingData, int len) {

    app::timestamp incomingMessage;
    memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

    reciever_buffer.push(incomingMessage);
}

void setupTransmitter() {

    reciever.init(message_received_callback);
}

void loopTransmitter() {
    if (mode == app::MODE::SYNC_CLOCKS) {

        infra::SyncClocks<
                            infra::ESPNowReceiver< app::timestamp >,
                            infra::ESPNowTransmitter< app::timestamp >
        > sync_clocks(reciever, transmitter, reciever_buffer);

        sync_clocks.start();

        mode = app::MODE::NORMAL_OPERATION;

    } else {

        auto rising_edge_timestamp = infra::wait_for_rising_edge();

        transmitter.send_message( rising_edge_timestamp );
    }
}

};