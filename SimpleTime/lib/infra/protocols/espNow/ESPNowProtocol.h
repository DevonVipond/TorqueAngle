#ifndef ESP_NOW_PROTOCOL_H
#define ESP_NOW_PROTOCOL_H

#include <functional>

#include <esp_now.h>
#include <WiFi.h>


namespace infra {

    struct ConnectionFailed : public std::exception
    {
        const char * what () const throw ()
        {
            return "Failed to init a connection";
        }
    };


    void init_protocol () {

        WiFi.mode(WIFI_STA);

        if (esp_now_init() != ESP_OK) {
            throw ConnectionFailed();
        }
    }


    static const uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

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

    template< typename PayloadType >
    class ESPNowReceiver {
        public:

        ESPNowReceiver() { }

        void init(void (* message_recieved_callback)( const unsigned char*, const unsigned char*, int ) ) {
            


            init_protocol();

            esp_now_register_recv_cb(message_recieved_callback);
        }

    };

};

#endif