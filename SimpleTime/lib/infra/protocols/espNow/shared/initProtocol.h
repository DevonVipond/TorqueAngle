
#pragma once

#include <esp_now.h>
#include <WiFi.h>
#include "../connectionFailedException.h"
#include "../../../logger/logger.h"


namespace infra {


    void init_protocol () {

        WiFi.mode(WIFI_STA);

        if (esp_now_init() != ESP_OK) {
            infra::log("init_protocol -> failed to initialize to wifi");
            throw ConnectionFailed();
        }

    }


};
