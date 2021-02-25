#ifndef ERRORVSRANGETEST_H
#define ERRORVSRANGETEST_H

#include <esp_now.h>
#include <WiFi.h>

namespace test
{
    namespace errorRateVsRange
    {
        namespace tx
        {
            uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

            //typedef struct struct_message
            //{
            //    char payload;
            //    //usigned int payload;
            //    //uint8_t payload;
            //    //uint16_t payload;
            //    //uint32_t payload;
            //} struct_message;

            //// Create a struct_message called myData
            //struct_message myData;

            // callback when data is sent
            void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
            {
                Serial.print("\r\nLast Packet Send Status:\t");
                Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
            }

            void setup()
            {
                // Init Serial Monitor
                Serial.begin(115200);

                // Set device as a Wi-Fi Station
                WiFi.mode(WIFI_STA);

                // Init ESP-NOW
                if (esp_now_init() != ESP_OK)
                {
                    Serial.println("Errorx` initializing ESP-NOW");
                    return;
                }

                // Once ESPNow is successfully Init, we will register for Send CB to
                // get the status of Trasnmitted packet
                esp_now_register_send_cb(OnDataSent);

                // Register peer
                esp_now_peer_info_t peerInfo;
                memcpy(peerInfo.peer_addr, broadcastAddress, 6);
                peerInfo.channel = 0;
                peerInfo.encrypt = false;

                // Add peer
                if (esp_now_add_peer(&peerInfo) != ESP_OK)
                {
                    Serial.println("Failed to add peer");
                    return;
                }
            }

            unsigned int counter =0; 
            void loop()
            {
                esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&counter, sizeof(counter));

                counter++;

                if (result == ESP_OK)
                {
                    Serial.println(millis());
                }
                else
                {
                    Serial.println("Error sending the data");
                }
                delay(15);
            }

        }

        namespace rx {
            typedef struct struct_message {
                unsigned int a;
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
            }

        };
    };
};

#endif