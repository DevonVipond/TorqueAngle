#pragma once

#include "./torque-angle-state.h"
#include "./clock-sync-state.h"
#include "../shared/message.h"
#include "../shared/communication-channel.h"

using namespace types;
using namespace shared;

namespace transmitter
{

    static const unsigned int NUMBER_MESSAGES_TO_TRANSMIT = 10;
    static const unsigned int TRANSMISSION_DELAY_MILLISECONDS = 40;
    static const unsigned int RESPONSE_TIMEOUT_MICROSECONDS = 5000;
    class EnterClockSyncMode
    {
        void execute(const CommunicationChannel &communicationChannel)
        {
            for (auto i = 0; i < NUMBER_MESSAGES_TO_TRANSMIT; i++)
            {

                communicationChannel.sendMessage(Message(Message::MESSAGE_TYPES::START_CLOCK_SYNC, 0));

                delay(TRANSMISSION_DELAY_MILLISECONDS);
            }
        }
    };

    class LeaveClockSyncMode
    {
        void execute(const CommunicationChannel &communicationChannel)
        {
            for (auto i = 0; i < NUMBER_MESSAGES_TO_TRANSMIT; i++)
            {

                communicationChannel.sendMessage(Message(Message::MESSAGE_TYPES::START_TORQUE_ANGLE_CALCULATION, 0));

                delay(TRANSMISSION_DELAY_MILLISECONDS);
            }
        }
    };

    class DetermineRoundTripTime
    {
    private:
        static const unsigned int RTT_SAMPLE_SIZE = 20;

        timestamp calculateRTT(CommunicationChannel &communicationChannel, const Clock &clock)
        {
            log("Sending Ping");

            auto startTime = clock.currentTime();

            communicationChannel.sendMessage(Message(Message::MESSAGE_TYPE::PING, startTime));

            try
            {
                Message response = communicationChannel.awaitNextResponse();
                auto endTime = clock.currentTime();
                if (startTime != response.payload())
                {
                    log("Received invalid packet!");
                    return NULL;
                }
                else
                {
                    auto rtt = endTime - startTime;
                    log("RTT took: ", rtt);

                    return rtt;
                }
            }
            catch (std::exception e)
            {
                log("RTT timeout!");
                return NULL;
            }
        }

        timestamp calculateAverateRTT(std::vector<timestamp> roundTripTimes)
        {

            if (roundTripTimes.empty())
                throw std::exception();

            std::sort(roundTripTimes.begin(), roundTripTimes.end());
            auto mid = roundTripTimes.size() / 2;

            return roundTripTimes[mid];
        }

        void execute(CommunicationChannel &communicationChannel, const Clock &clock) const
        {

            std::vector<timestamp> roundTripTimes;

            auto shouldPing = [&roundTripTimes]() { return roundTripTimes.size() < RTT_SAMPLE_SIZE };

            while (shouldPing())
            {
                auto rtt = calculateRTT(communicationChannel);
                roundTripTimes.push_back(rtt);

                delay(TRANSMISSION_DELAY_MILLISECONDS);
            }

            return calculateAverateRTT(roundTripTimes);
        }
    };

    class SyncReceiver
    {
        void execute(timestamp expectedRTT, CommunicationChannel &communicationChannel, const Clock &clock))
        {
            for (int i = 0; i < NUMBER_MESSAGES_TO_TRANSMIT; i++)
            {

                timestamp oneWayTripTime = expectedRTT / 2;
                timestamp syncTime = clock.currentTime() + oneWayTripTime;

                communicationChannel.sendMessage(Message(Message::MESSAGE_TYPES::SYNC_CLOCK_MESSAGE, syncTime));

                delay(TRANSMISSION_DELAY_MILLISECONDS);
            }
        }
    };

    class ClockSyncState 
    {

    public:
        ClockSyncState() = delete;
        ClockSyncState(const CommunicationChannel &communicationChannel) : _communicationChannel(communicationChannel)
        {
        }

        void start()
        {
            Clock clock;

            log("Commanding rx to enter sync clocks mode");
            EnterClockSyncMode cmd1;
            cmd1.execute(_communicationChannel);

            log("Calculating trip time");
            DetermineRoundTripTime cmd2;
            timestamp averageRTT = cmd2.execute(_communicationChannel, clock);

            log("Commanding rx to sync his clock to my clock");

            SyncReceiver cmd3;
            cmd3.execute(averageRTT, _communicationChannel, clock);

            log("Clock sync successful. Commanding rx to enter torque angle mode");

            LeaveClockSyncMode cmd4;
            cmd4.execute(_communicationChannel);

            log("Leaving clock sync mode");
        }

    private:
        CommunicationChannel &_communicationChannel;
    };
}
}