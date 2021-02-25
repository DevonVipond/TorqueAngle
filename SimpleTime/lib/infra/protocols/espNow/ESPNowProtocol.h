#ifndef ESP_NOW_PROTOCOL_H
#define ESP_NOW_PROTOCOL_H

#include <esp_now.h>
#include <WiFi.h>

#include "../../message/message.h"

namespace infra {

    template< typename PayloadType >
    class Protocol {
        public:

        Protocol() {
            uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

            // Set device as a Wi-Fi Station
            WiFi.mode(WIFI_STA);

            // Init ESP-NOW
            if (esp_now_init() != ESP_OK) {
                Serial.println("Error` initializing ESP-NOW");
                return;
            }
            
            // Register peer
            esp_now_peer_info_t peerInfo;
            memcpy(peerInfo.peer_addr, broadcastAddress, 6);
            peerInfo.channel = 0;  
            peerInfo.encrypt = false;
            
            // Add peer        
            if (esp_now_add_peer(&peerInfo) != ESP_OK){
                Serial.println("Failed to add peer");
                return;
            }
        }

        void send_message(infra::Message<PayloadType> message) {
            uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

            PayloadType payload = message.get_payload();
            size_t size = message.size();
            esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &payload, size);
        }
    };

};

#endif