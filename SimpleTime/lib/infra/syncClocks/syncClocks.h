#pragma once

#include <queue>
#include <exception>
#include <vector>
#include <algorithm>

#include "../time/getCurrentTime.h"
#include "../../app/types/types.h"
#include "../../app/message/message.h"
#include "../logger/logger.h"

namespace infra {

    template< typename Receiver, typename Transmitter >
    class SyncClocks {

        public:

        static const app::timestamp NUMBER_MESSAGES_TO_TRANSMIT = 10;
        static const app::timestamp TRANSMISSION_DELAY_MILLISECONDS = 600;
        static const app::timestamp RESPONSE_TIMEOUT_MICROSECONDS = 600000;

        Receiver &_receiver;
        Transmitter &_transmitter;
        std::queue<app::timestamp> &_receiver_buffer;

        SyncClocks() = delete;
        SyncClocks(Receiver &receiver, Transmitter &transmitter, std::queue<app::timestamp> &receiver_buffer):
            _receiver(receiver),
            _transmitter(transmitter),
            _receiver_buffer(receiver_buffer) { }

        void start(){

            infra::log("commanding rx to enter sync clocks mode");

            initiate_sync_clock_mode();

            infra::log("calculating trip time");

            app::timestamp average_trip_time = calculate_trip_time();

            infra::log("commanding rx to sync his clock to my clock");

            send_clock_sync_messages(average_trip_time);

            infra::log("clock sync successful. commanding rx to enter torque angle mode");

            finish_clock_sync_mode();
            
            infra::log("leaving clock sync mode");
        }

        private:

        class RoundTrip {

            public:
            app::timestamp start_time;
            app::timestamp end_time;

            RoundTrip( app::timestamp start_time, app::timestamp end_time ):
                start_time(start_time), end_time(end_time) {};
        };


        void initiate_sync_clock_mode() {

            for (int i= 0; i < NUMBER_MESSAGES_TO_TRANSMIT; i++) {

                _transmitter.send_message( app::enter_sync_clocks_mode_message() );

                delay(TRANSMISSION_DELAY_MILLISECONDS);

            }

            // Assume reciever is now in sync clock mode
        }

        app::timestamp calculate_trip_time() {

            std::vector<app::timestamp> times;
            int no_failure = 0;

            while (times.size() < NUMBER_MESSAGES_TO_TRANSMIT) {
                infra::log("calculate_trip_time -> sending round trip message");
                
                auto start_time = get_current_time();

                _transmitter.send_message( app::round_trip_message(start_time) );

                try {
                    app::timestamp response = wait_for_response();
                    app::timestamp end_time = get_current_time();

                    if (response != start_time) {
                        log("calculate_trip_time -> received invalid response");
                        throw std::exception();
                    }


                    Serial.print("trip time took: ");
                    Serial.print(end_time - start_time);

                    times.push_back(end_time - start_time);

                } catch (const std::exception & e) {

                    no_failure++;

                    if (no_failure >= NUMBER_MESSAGES_TO_TRANSMIT) {
                        log("failed to calc trip time due to max no error msg's exceeded");
                        throw std::exception();
                    }

                    infra::log("failed to calculate trip time. retrying");

                }


                delay(TRANSMISSION_DELAY_MILLISECONDS);
            }


            return calculate_average_trip_time(times);
        }

        app::timestamp wait_for_response() {
                
            auto timeout = get_current_time() + RESPONSE_TIMEOUT_MICROSECONDS;

            while ( _receiver_buffer.size() ) _receiver_buffer.pop();

            while ( get_current_time() < timeout ) {

                if (_receiver_buffer.size()) {
                    auto ret = _receiver_buffer.front(); _receiver_buffer.pop();

                    return ret;
                }
            }

            infra::log("wait_for_response -> timeout exceeded");
            throw std::exception();

        }

        void send_clock_sync_messages(app::timestamp average_trip_time) {

            for (int i = 0; i < NUMBER_MESSAGES_TO_TRANSMIT; i++) {

                app::timestamp current_time = get_current_time();

                _transmitter.send_message( app::sync_clock_message( current_time + average_trip_time) );

                delay(TRANSMISSION_DELAY_MILLISECONDS);

            }

        }

        void finish_clock_sync_mode() {

            for (int i = 0; i < NUMBER_MESSAGES_TO_TRANSMIT; i++) {

                _transmitter.send_message( app::enter_torque_angle_mode_message() );

                delay(TRANSMISSION_DELAY_MILLISECONDS);

            }

        }

        app::timestamp calculate_average_trip_time( std::vector<app::timestamp> two_way_trip_times){
            #define DATAPOINTS_TO_DISCARD 4
            std::sort(two_way_trip_times.begin(), two_way_trip_times.end());

            std::vector<app::timestamp> times(two_way_trip_times.size()-4);

            std:copy(&two_way_trip_times[DATAPOINTS_TO_DISCARD/2], &two_way_trip_times[two_way_trip_times.size()-2], &times[0]);

            app::timestamp sum_trip_time = 0;

            for (const auto trip_time : times) {

                sum_trip_time += trip_time;
            }

            auto average_two_way_trip_time = sum_trip_time / times.size();

            return average_two_way_trip_time / 2;

        }

    };
}
