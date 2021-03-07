
#pragma once

#include <esp_now.h>
#include <WiFi.h>
#include "../connectionFailedException.h"


namespace infra {


    void init_protocol () {

        //if (app::Globals::instance()->g_protocol_initialized) return;

        //app::Globals::instance()->g_protocol_initialized)= true;


        WiFi.mode(WIFI_STA);

        if (esp_now_init() != ESP_OK) {
            throw ConnectionFailed();
        }

    }


};
