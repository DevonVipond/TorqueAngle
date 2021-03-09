#pragma once

#include <string>

namespace infra {
    void log(std::string s) {

        Serial.println(s.c_str());

    }
}