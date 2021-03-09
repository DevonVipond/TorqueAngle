
#pragma once

#include "shared/constants.h"
#include "shared/initProtocol.h"
#include "connectionFailedException.h"
#include "../../../infra/logger/logger.h"

#include <esp_now.h>
#include <WiFi.h>


namespace infra {

    template< typename PayloadType >
    class ESPNowTransmitter {
    private:

        bool _initialized;
        const uint8_t* _broadcast_addr;

        void __init(const uint8_t *broadcast_addr) {
            _broadcast_addr = broadcast_addr;

            esp_now_peer_info_t peerInfo;
            memcpy(peerInfo.peer_addr, broadcast_addr, 6);
            peerInfo.channel = 0;
            peerInfo.encrypt = false;

            // Add peer
            if (esp_now_add_peer(&peerInfo) != ESP_OK){
                log("ESPNowTransmitter::__init -> Failed to add peer!");
                throw ConnectionFailed();
            }

            _initialized = true;

        }


    public:
        void init(const uint8_t* broadcast_addr, void (* message_recieved_callback)( const uint8_t *mac_addr, esp_now_send_status_t status ) ) {

            esp_now_register_send_cb(message_recieved_callback);

            __init(broadcast_addr);
        }

        void init(const uint8_t *broadcast_addr){
            __init(broadcast_addr);
        }


        ESPNowTransmitter(): _initialized(false) { }

        void send_message(PayloadType payload) {
            if (!_initialized) {
                log("ESPNowTransmitter::send_message -> transmitter not initialized!");
                throw ConnectionFailed();
            }

            esp_err_t result = esp_now_send(_broadcast_addr, (uint8_t *) &payload, sizeof(PayloadType));
        }
    };

};
