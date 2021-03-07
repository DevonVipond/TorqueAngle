
#pragma once

#include <esp_now.h>
#include <WiFi.h>
#include "../espNowReciever.h"
#include "../connectionFailedException.h"


namespace infra {


    void init_protocol () {
        if (ESPNowReceiver<app::payload>::initialized) return;

        ESPNowReceiver<app::payload>::initialized = true;


        WiFi.mode(WIFI_STA);

        if (esp_now_init() != ESP_OK) {
            throw ConnectionFailed();
        }

        already_init = true;
    }


};
