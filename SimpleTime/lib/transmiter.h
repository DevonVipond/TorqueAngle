#pragma once

#include <queue>

#include "app/types/types.h"
#include "app/message/message.h"
#include "infra/protocols/espNow/espNowTransmitter.h"
#include "infra/protocols/espNow/espNowReciever.h"
#include "infra/protocols/espNow/shared/constants.h"
#include "infra/sensor/readSenserData.h"
#include "infra/readNumberBolts/readNumberBolts.h"
#include "infra/syncClocks/syncClocks.h"
#include "infra/logger/logger.h"
#include "infra/protocols/espNow/shared/initProtocol.h"
# define ZERO_CLOCK_PIN 11

namespace transmiter {

infra::ESPNowTransmitter< app::message > transmitter;
//infra::ESPNowReceiver< app::message > reciever;
unsigned int number_bolts;

//std::queue<app::timestamp> reciever_buffer;

//app::CURRENT_MODE mode = app::SYNC_CLOCKS_MODE;

//void message_received_callback(const uint8_t *mac, const uint8_t *incomingData, int len) {
//
//    app::message incomingMessage;
//    memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));
//
//    Serial.print("tx: recieved message");
//    Serial.print(incomingMessage.payload);
//    Serial.println(incomingMessage.message_type);
//
//    //reciever_buffer.push(incomingMessage.payload);
//}

void message_sent_callback(const uint8_t *mac_addr, esp_now_send_status_t status) {

  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

}


void setupTransmitter() {
    try {

        infra::init_protocol();

        transmitter.init(infra::rx_broadcastAddress, message_sent_callback);
        //reciever.init(message_received_callback);

        Serial.begin(115200);

        pinMode(app::TERMINAL_VOLTAGE_PIN, INPUT);    
        pinMode(app::NUMBER_BOLTS_PIN, INPUT);    


        number_bolts = infra::read_number_bolts();;
        delay (100);


    } catch (std::exception e) {

        Serial.print("EXCEPTION: ");

        Serial.println(e.what());

        throw e;

    }
}



unsigned int bolts_counter = 0;
void loopTransmitter() {
    try {

        //if (mode == app::SYNC_CLOCKS_MODE) {
        //    using R = infra::ESPNowReceiver< app::message >;
        //    using T = infra::ESPNowTransmitter< app::message >;

        //    //infra::SyncClocks< R, T > sync_clocks(reciever, transmitter, reciever_buffer);

        //    //sync_clocks.start();

        //    mode = app::TORQUE_ANGLE_MODE;

        //} else if (mode == app::TORQUE_ANGLE_MODE) {

            auto rising_edge_timestamp = infra::wait_for_rising_edge(app::SENSOR_PIN);

            bolts_counter = (bolts_counter + 1) % number_bolts;

            if (bolts_counter == 0) {
                transmitter.send_message( app::sensor_timestamp_message(rising_edge_timestamp) );
            }

            delayMicroseconds(30);
        //}

    } catch (std::exception e) {
        Serial.print("EXCEPTION: ");
        Serial.println(e.what());

        throw e;
    }

}

};