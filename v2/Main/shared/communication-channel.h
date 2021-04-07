#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <esp_now.h>
#include <WiFi.h>
#include "../shared/logger.h"
#include "../shared/message.h"
#include "../shared/observable.h"

using namespace std::placeholders;

namespace shared
{
    class AwaitNextResponse : public Observer
    {
    private:
        Message _response;
        bool _doneWaiting = false;

    public:
        AwaitNextResponse() {}

        void observerCallback(Message message)
        {
            _response = message;
            _doneWaiting = true;
        }

        bool doneWaiting()
        {
            return _doneWaiting;
        }

        Message getResponse()
        {
            return _response;
        }
    };

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
        uint8_t *_deviceAddress;
        uint8_t *_receiverAddress;

    public:
        static std::vector<Observer *> &subscribers()
        {
            static std::vector<Observer *> __subs;
            return __subs;
        }

        static void callbackHACK(const uint8_t *mac, const uint8_t *incomingData, int len) // Must be static because this function cannot have an implicit this pointer
        {

            Message incomingMessage;
            memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

            log("Received message with payload: ", incomingMessage.payload());

            for (Observer *itr : CommunicationChannel::subscribers())
            {
                itr->observerCallback(incomingMessage);
            }
        }

        CommunicationChannel(uint8_t deviceAddress[], uint8_t receiverAddress[]) : _deviceAddress(deviceAddress),
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

            esp_now_register_recv_cb(callbackHACK);
        }

        Message awaitNextResponse(timestamp timeoutMicroseconds = 5000 * 1000)
        {

            AwaitNextResponse awaitNextResponse;

            subscribe(&awaitNextResponse);

            Timer timeoutTimer(timeoutMicroseconds);
            timeoutTimer.start();

            while (!awaitNextResponse.doneWaiting() || !timeoutTimer.isFinished())
            {
                delayMicroseconds(10);
            }

            unsubscribe(&awaitNextResponse);

            if (timeoutTimer.isFinished())
            {
                log("Timeout exceeded in awaitNextResponse");
                throw std::exception();
            }

            return awaitNextResponse.getResponse();
        }

        void sendMessage(Message message) const
        {
            esp_err_t result = esp_now_send(_receiverAddress, (uint8_t *)&message, sizeof(Message));
        }

        void subscribe(Observer *observer)
        {
            CommunicationChannel::subscribers().push_back(observer);
        }

        void unsubscribe(Observer *observer)
        {
            auto itr = std::find(begin(CommunicationChannel::subscribers()), end(CommunicationChannel::subscribers()), observer);

            if (itr == CommunicationChannel::subscribers().end())
            {
                log("unsubscribe: Invalid Observer!");
                throw std::exception();
            }

            CommunicationChannel::subscribers().erase(itr);
        }
    };
}