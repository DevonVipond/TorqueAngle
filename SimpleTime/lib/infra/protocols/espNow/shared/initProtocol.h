
#pragma once

#include <esp_now.h>
#include <WiFi.h>


namespace infra {


    void init_protocol () {

        WiFi.mode(WIFI_STA);

        if (esp_now_init() != ESP_OK) {
            throw ConnectionFailed();
        }
    }


};
