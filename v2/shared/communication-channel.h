#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <esp_now.h>
#include <WiFi.h>
#include "../shared/logger.h"

namespace shared
{

    using SUBSCRIBER_ID = unsigned int;

    class Timer
    {
    private:
        Clock _clock;
        timestamp _timeInterval;
        timestamp _startTime;

    public:
        Timer(timestamp timeIntervalMicroseconds) : _clock(Clock()),
                                        _timeInterval(timeIntervalMicroseconds),
                                        _startTime(NULL) {}

        void start()
        {
            _startTime = _clock.currentTime();
        }

        void reset()
        {
            _startTime = NULL;
        }

        bool isFinished() const { return (_startTime != NULL && _clock.currentTime() > _startTime + _timeInterval) ? true : false; }
    };

    class CommunicationChannel
    {
    private:
        const uint8_t _deviceAddress[];
        const uint8_t _receiverAddress[];

        std::unordered_map<SUBSCRIBER_ID, std::function<void(Message)>> _subscribers;
        SUBSCRIBER_ID _nextSubscriberId(1);

        void messageReceivedCallback(const uint8_t *mac, const uint8_t *incomingData, int len)
        {

            Message incomingMessage;
            memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

            log("Received message with payload: ", incomingMessage.payload());

            for (const auto &itr : _subscribers)
            {
                itr->second(incomingMessage);
            }
        }

    public:
        CommunicationChannel(const uint8_t deviceAddress[], const uint8_t receiverAddress[]) : _deviceAddress(deviceAddress),
                                                                                               _receiverAddress(receiverAddress)
        {
            WiFi.mode(WIFI_STA);

            if (esp_now_init() != ESP_OK)
            {
                log("Error initializing ESP-NOW");
                return;
            }

            esp_now_peer_info_t peerInfo;
            memcpy(peerInfo.peer_addr, _deviceAddress, 6);
            peerInfo.channel = 0;
            peerInfo.encrypt = false;

            if (esp_now_add_peer(&peerInfo) != ESP_OK)
            {
                log("Failed to add peer");
                return;
            }

            esp_now_register_recv_cb(messageReceivedCallback);
        }

        Message awaitNextResponse(timestamp timeoutMicroseconds = 5000 * 1000)
        {

            volatile bool &hasCallbackFired = false;
            volatile Message response;

            auto responseCallback = [&hasCallbackFired, &response](Message m) {
                response = m;
                hasCallbackFired = true;
            };

            Timer timeoutTimer(timeoutMicroseconds);
            timeoutTimer.start();

            auto subscriberId = subscribe(responseCallback);

            while (!hasCallbackFired || !timeoutTimer.isFinished())
            {
                delayMicroseconds(10);
            }

            unsubscribe(subscriberId);

            if (timeoutTimer.isFinished())
            {
                log("Timeout exceeded in awaitNextResponse");
                throw std::exception();
            }

            return response;
        }

        void sendMessage(Message message) const
        {
            esp_err_t result = esp_now_send(_receiverAddress, (uint8_t *)&message, sizeof(Message));
        }

        SUBSCRIBER_ID subscribe(std::function<void(Message)> callback)
        {
            auto subscriberId = _nextSubscriberId;

            _subscribers[subscriberId] = callback;

            _nextSubscriberId++;
            return subscriberId;
        }

        void unsubscribe(SUBSCRIBER_ID subscriberId)
        {
            auto callback = _subscribers.find(subscriberId);

            if (callback == subscriberId.end())
            {
                log("Invalid subscriberId: ", subscriberId, " in unsubscribe");
                throw std::exception();
            }

            _subscribers.erase(callback)
        }
    };

}