#ifndef LIBRX_H 
#define LIBRX_H

#include <esp_now.h>
#include <WiFi.h>

#include "../app/buffer/buffer.h"
#include "../app/types/types.h"

namespace rx {
    typedef struct struct_message {
    unsigned long a;
    } struct_message;
    
    // Create a struct_message called myData
    struct_message myData;
    
    // callback function that will be executed when data is received
    void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
      memcpy(&myData, incomingData, sizeof(myData));
    
      Serial.println(myData.a);
      Serial.println();
    }
     
    void setup() {
        // Initialize Serial Monitor
        Serial.begin(115200);
        
        // Set device as a Wi-Fi Station
        WiFi.mode(WIFI_STA);
    
        // Init ESP-NOW
        if (esp_now_init() != ESP_OK) {
          Serial.println("Error initializing ESP-NOW");
          return;
        }
        
        // Once ESPNow is successfully Init, we will register for recv CB to
        // get recv packer info
        esp_now_register_recv_cb(OnDataRecv);

        Buffer::Buffer<types::timestamp> buffer(1000);
        Serial.println("my buffer did not blow up!");
    }

    void loop() { }
};

#endif