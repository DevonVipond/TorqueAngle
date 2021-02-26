
#include "lib/app/types/types.h"
#include "lib/infra/protocols/espNow/espNowTransmitter.h"
#include "lib/infra/sensor/readSenserData.h"

infra::ESPNowTransmitter< app::timestamp > transmitter;

void loopTransmitter() {

    auto rising_edge_timestamp = infra::wait_for_rising_edge();

    transmitter.send_message( rising_edge_timestamp );

    //delay(DELAY_PERIOD);
}

