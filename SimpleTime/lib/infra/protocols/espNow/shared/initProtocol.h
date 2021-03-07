
#pragma once

#include <esp_now.h>
#include <WiFi.h>


namespace infra {


    void init_protocol () {
        static bool already_init = false;

        if (already_init) return;


        WiFi.mode(WIFI_STA);

        if (esp_now_init() != ESP_OK) {
            throw ConnectionFailed();
        }

        already_init = true;
    }


};
