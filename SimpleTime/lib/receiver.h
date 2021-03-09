#pragma once

#include <queue>
#include <memory>

#include "app/types/types.h"
#include "app/globalVariables/globalVariables.h"
#include "app/message/message.h"
#include "infra/protocols/espNow/espNowReciever.h"
#include "infra/protocols/espNow/espNowTransmitter.h"
#include "infra/sensor/readSenserData.h"
#include "infra/terminalVoltage/readTerminalVoltage.h"
#include "infra/protocols/espNow/shared/constants.h"
#include "app/torqueAngle/calculateTorqueAngle.h"
#include "infra/syncClocks/syncClocks.h"

namespace receiver {

infra::ESPNowReceiver<app::message> receiver;
infra::ESPNowTransmitter< app::message > transmitter;

std::queue<app::timestamp> receiver_buffer;
std::queue<app::timestamp> terminal_voltage_zero_crossings_buffer;

app::CURRENT_MODE mode = app::SYNC_CLOCKS_MODE;

app::timestamp no_load_time_shift = 0;

#define ROTOR_FREQUENCY 3600

using namespace app;
using namespace infra;

void message_sent_callback(const uint8_t *mac_addr, esp_now_send_status_t status) {

  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

}

void display_torque_angle(app::TorqueAngle torque_angle) {

    Serial.print("torque angle: ");
    Serial.println(torque_angle);
}

void update_torque_angle() {

    //auto torque_angle = app::calculate_average_torque_angle(receiver_buffer, terminal_voltage_zero_crossings_buffer, no_load_time_shift);
    try {
        auto reference_point = receiver_buffer.front(); receiver_buffer.pop();
        while (receiver_buffer.size()) receiver_buffer.pop();
        auto torque_angle = app::__calculate_torque_angle(reference_point,
                                        terminal_voltage_zero_crossings_buffer,
                                        no_load_time_shift,
                                        ROTOR_FREQUENCY);


        display_torque_angle(torque_angle);
    } catch (const std::exception& e) {

        Serial.print("Exception in update_torque_angle ");
        Serial.println(e.what());

    }


    //while (terminal_voltage_zero_crossings_buffer.size()) terminal_voltage_zero_crossings_buffer.pop();
    //while (receiver_buffer.size()) receiver_buffer.pop();
}

void zero_torque_angle(app::timestamp no_load_time_shift) {

    no_load_time_shift = no_load_time_shift;

}

void set_clock_offset(const app::timestamp &transmitter_clock) {

    g_clock_offset = static_cast<long int>(transmitter_clock) - static_cast<long int>(micros());

    Serial.print("clock syncd w offset: ");
    Serial.println(g_clock_offset);

}

std::string msg_type_to_string(app::MESSAGE_TYPE type) {
    switch (type) {
        case app::ROUND_TRIP_MSG:             
            return "ROUND_TRIP_MSG";
        case app::SYNC_CLOCK_MSG:            
            return "SYNC_CLOCK_MSG";
        case app::SENSOR_TIMESTAMP_MSG:
            return "SENSOR_TIMESTAMP_MSG";
        case app::ENTER_SYNC_CLOCKS_MODE_MSG:
            return "ENTER_SYNC_CLOCKS_MODE_MSG";
        case app::ENTER_TORQUE_ANGLE_MODE_MSG:
            return "ENTER_TORQUE_ANGLE_MODE_MSG";
    }

    return "undefined type!";
}

void message_handler(app::message & msg) {
    auto message_type = msg.message_type;
    if (msg.message_type == app::ENTER_SYNC_CLOCKS_MODE_MSG) {

        mode = app::SYNC_CLOCKS_MODE;
        while (terminal_voltage_zero_crossings_buffer.size()) terminal_voltage_zero_crossings_buffer.pop();
        while (receiver_buffer.size()) receiver_buffer.pop();
    } else if (message_type == app::ENTER_TORQUE_ANGLE_MODE_MSG)  {

        mode = app::TORQUE_ANGLE_MODE;
        while (terminal_voltage_zero_crossings_buffer.size()) terminal_voltage_zero_crossings_buffer.pop();
        while (receiver_buffer.size()) receiver_buffer.pop();

    } else if( message_type == app::ROUND_TRIP_MSG) {

        if (mode != app::SYNC_CLOCKS_MODE) {
            throw std::exception();
        }

        transmitter.send_message( round_trip_message(msg.payload) );

    } else if( message_type == app::SYNC_CLOCK_MSG) {

        if (mode != app::SYNC_CLOCKS_MODE) {
            Serial.println("received sync_clock_message when in torque angle mode!");
            throw std::exception();
        }

        set_clock_offset(msg.payload);

        Serial.print("Syncing clock to: ");
        Serial.println(msg.payload);

    } else if( message_type == app::SENSOR_TIMESTAMP_MSG) {

        if (mode != app::TORQUE_ANGLE_MODE) {
            throw std::exception();
        }

        receiver_buffer.push(msg.payload);
    } else {

        Serial.println("Unable to handle message");
        throw std::exception();
    }

    //Serial.print("rx: received msg w payload: ");
    //Serial.print(msg.payload);
    //Serial.print(", type: ");
    //Serial.println(msg_type_to_string(msg.message_type).c_str());
    //Serial.println(msg.message_type);


}

void message_received_callback(const uint8_t *mac, const uint8_t *incomingData, int len) {

    app::message incoming_message;
    memcpy(&incoming_message, incomingData, sizeof(incoming_message));

    message_handler(incoming_message);
}

void setupReciever() {

    Serial.begin(115200);

    uint8_t broadcastAddress[] = { 0x3c,0x61,0x05,0x3e,0xee,0xa4 };

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

    //try {

    //    Serial.begin(115200);

    //    infra::init_protocol();

    //    transmitter.init(infra::tx_broadcastAddress, message_sent_callback);
    //    receiver.init(message_received_callback);

    //    pinMode(app::SENSOR_PIN, INPUT);    


    //} catch (const std::exception &e) {

    //    Serial.print("exception in setupReceiver: ");

    //    Serial.println(e.what());

    //    return;
    //}

}

void loopReciever() {
    while (1) {
        try {

            if (mode == app::TORQUE_ANGLE_MODE) {

                auto zero_crossing_timestamp = infra::wait_for_rising_edge(app::TERMINAL_VOLTAGE_PIN);

                terminal_voltage_zero_crossings_buffer.push(zero_crossing_timestamp);

                if ( receiver_buffer.size() > 50 )
                    update_torque_angle();
            }
            else {

                delayMicroseconds(20);

            }
        } catch (const std::exception &e) {
            Serial.print("exception in loopReciever: ");

            Serial.println(e.what());

            return;
        }

    }


}

};