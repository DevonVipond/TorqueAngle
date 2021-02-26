
#pragma once

#include "shared/constants.h"
#include "shared/initProtocol.h"

#include <esp_now.h>
#include <WiFi.h>


namespace infra {

    template< typename PayloadType >
    class ESPNowTransmitter {
    private:

        bool _initialized;

        // ESP protocol is initialized when needed (lazy initialization)
        void init() {

            init_protocol();

            esp_now_peer_info_t peerInfo;
            memcpy(peerInfo.peer_addr, broadcastAddress, 6);
            peerInfo.channel = 0;
            peerInfo.encrypt = false;

            // Add peer
            if (esp_now_add_peer(&peerInfo) != ESP_OK){
                return;
            }

            _initialized = true;

        }

    public:

        ESPNowTransmitter(): _initialized(false) { }

        void send_message(PayloadType payload) {
            if (!_initialized) {
                init();
            }

            esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &payload, sizeof(PayloadType));
        }
    };

};
