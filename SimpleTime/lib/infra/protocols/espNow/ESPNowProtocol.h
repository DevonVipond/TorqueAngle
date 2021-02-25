#ifndef ESP_NOW_PROTOCOL_H
#define ESP_NOW_PROTOCOL_H

#include <esp_now.h>
#include <WiFi.h>

#include "../../message/message.h"

namespace infra {

    struct ConnectionFailed : public std::exception
    {
        const char * what () const throw ()
        {
            return "Failed to init a connection";
        }
    }


    void init_protocol () {

        WiFi.mode(WIFI_STA);

        if (esp_now_init() != ESP_OK) {
            throw ConnectionFailed;
        }
    }


    static const uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    template< typename PayloadType >
    class ESPNowTransmitter {
    private:

        bool _initialized;

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

    template< typename PayloadType >
    class ESPNowReciever {
        public:

        ESPNowReciever() { }

        void init(std::function<void(const uint8_t * mac, const uint8_t *incomingData, int len)> message_recieved_callback) {

            init_protocol();

            esp_now_register_recv_cb(message_recieved_callback);
        }

    };

};

#endif