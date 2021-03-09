
#pragma once

#include "shared/constants.h"
#include "shared/initProtocol.h"
#include "../../../infra/logger/logger.h"

#include <esp_now.h>
#include <WiFi.h>


namespace infra {

    template< typename PayloadType >
    class ESPNowReceiver {
        public:

        ESPNowReceiver() { }

        void init(void (* message_recieved_callback)( const unsigned char*, const unsigned char*, int ) ) {

            esp_now_register_recv_cb(message_recieved_callback);
        }

    };

};
