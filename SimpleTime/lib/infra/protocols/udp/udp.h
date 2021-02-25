#ifndef UDP_H
#define UDP_H

#include "WiFi.h"
#include "AsyncUDP.h"

namespace udp {

    class Server {
        private:
            AsyncUDP udp;
            void _setup(const char * ssid, const char * password) {
                Serial.begin(115200);
                WiFi.mode(WIFI_STA);
                WiFi.begin(ssid, password);
                if (WiFi.waitForConnectResult() != WL_CONNECTED) {
                    Serial.println("WiFi Failed");
                    while(1) {
                        delay(1000);
                    }
                }
                if(udp.listen(1234)) {
                    Serial.print("UDP Listening on IP: ");
                    Serial.println(WiFi.localIP());
                    udp.onPacket([](AsyncUDPPacket packet) {
                        Serial.print("UDP Packet Type: ");
                        Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
                        Serial.print(", From: ");
                        Serial.print(packet.remoteIP());
                        Serial.print(":");
                        Serial.print(packet.remotePort());
                        Serial.print(", To: ");
                        Serial.print(packet.localIP());
                        Serial.print(":");
                        Serial.print(packet.localPort());
                        Serial.print(", Length: ");
                        Serial.print(packet.length());
                        Serial.print(", Data: ");
                        Serial.write(packet.data(), packet.length());
                        Serial.println();
                        //reply to the client
                        packet.printf("Got %u bytes of data", packet.length());
                    });
                }
            }

            void _loop() {
                delay(1000);
                //Send broadcast
                udp.broadcast("Anyone here?");
            }

        public:
            Server() {}

            void run(const char * ssid, const char * password){
                _setup(const char * ssid, const char * password);
                _loop();
            }
    };


}

#endif