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

namespace transmiter {

infra::ESPNowTransmitter< app::message > transmitter;
infra::ESPNowReceiver< app::message > reciever;
unsigned int number_bolts;

std::queue<app::timestamp> reciever_buffer;

app::CURRENT_MODE mode = app::SYNC_CLOCKS_MODE;

void message_received_callback(const uint8_t *mac, const uint8_t *incomingData, int len) {

    app::message incomingMessage;
    memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

    Serial.print("tx: recieved message");
    Serial.print("payload");
    Serial.print(incomingMessage.payload);
    Serial.print(", type: ");
    Serial.println(incomingMessage.message_type);

    reciever_buffer.push(incomingMessage.payload);
}

void message_sent_callback(const uint8_t *mac_addr, esp_now_send_status_t status) {

  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

}


void setupTransmitter() {
    Serial.begin(115200);

    uint8_t broadcastAddress[] = {0x3C, 0x61, 0x05, 0x3E, 0xD7, 0x4C};

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }
    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(message_received_callback);
    esp_now_register_send_cb(message_sent_callback);
    number_bolts = infra::read_number_bolts();;
    //try {

    //    Serial.begin(115200);

    //    infra::init_protocol();

    //    transmitter.init(infra::rx_broadcastAddress, message_sent_callback);
    //    reciever.init(message_received_callback);


    //    pinMode(app::TERMINAL_VOLTAGE_PIN, INPUT);    
    //    pinMode(app::NUMBER_BOLTS_PIN, INPUT);    


    //    number_bolts = infra::read_number_bolts();;
    //    delay (100);


    //} catch (std::exception e) {

    //    Serial.print("exception in setupTransmitter: ");

    //    Serial.println(e.what());

    //    return;

    //}
}



unsigned int bolts_counter = 0;
unsigned int sync_clock_counter = 0;
void loopTransmitter() {
    while (1) {
        try {

            if (mode == app::SYNC_CLOCKS_MODE) {
                using R = infra::ESPNowReceiver< app::message >;
                using T = infra::ESPNowTransmitter< app::message >;

                infra::SyncClocks< R, T > sync_clocks(reciever, transmitter, reciever_buffer);

                sync_clocks.start();

                mode = app::TORQUE_ANGLE_MODE;

                delayMicroseconds(100);

            } else if (mode == app::TORQUE_ANGLE_MODE) {

                auto rising_edge_timestamp = infra::wait_for_rising_edge(app::SENSOR_PIN);

                bolts_counter = (bolts_counter + 1) % number_bolts;

                if (bolts_counter == 0) {
                    Serial.println("sending rising edge timestamp");
                    transmitter.send_message( app::sensor_timestamp_message(rising_edge_timestamp) );
                }

                sync_clock_counter++;
                if (sync_clock_counter >= 10000) {

                    sync_clock_counter = 0;
                    mode = app::SYNC_CLOCKS_MODE;

                }

                delayMicroseconds(25);
            }

        } catch (std::exception e) {
            Serial.print("exception in loopTransmitter: ");

            Serial.println(e.what());

            return;
        }
    }

}

};