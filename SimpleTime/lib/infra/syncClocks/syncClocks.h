#pragma once

#include <queue>
#include <exception>

#include "../time/getCurrentTime.h"
#include "../../app/types/types.h"

namespace infra {

    template< typename Protocol >
    class SyncClocksTransmitter {

        public:

        static const app::timestamp INITIATE_CLOCK_SYNC_MODE_MESSAGE = -1;
        static const app::timestamp FINISH_CLOCK_SYNC_MODE_MESSAGE = -1;
        static const app::timestamp NUMBER_SYNC_CLOCK_MESSAGES = 20;
        static const app::timestamp TRANSMISSION_DELAY = 200;
        static const app::timestamp RESPONSE_TIMEOUT_MICROSECONDS = 2000;

        Protocol &_protocol;
        std::queue<app::timestamp> &_receiver_buffer;

        SyncClocks() = delete;
        SyncClocks(Protocol &protocol, std::queue<app::timestamp> &receiver_buffer):
            _protocol(protocol),
            _receiver_buffer(receiver_buffer) { }

        void start(){

            initiate_sync_clock_mode();

            app::timestamp average_trip_time = calculate_trip_time();

            send_clock_sync_messages(average_trip_time);

            finish_clock_sync_mode();
        }

        private:

        class RoundTrip {
            public:

            app::timestamp start_time;
            app::timestamp end_time;

            RoundTrip( app::timestamp start_time, app::timestamp end_time):
                start_time(start_time), end_time(end_time);
        };


        void initiate_sync_clock_mode() {

            for (int i= 0; i < NUMBER_SYNC_CLOCK_MESSAGES; i++) {

                protocol.send_message(INITIATE_CLOCK_SYNC_MODE);

                delay(TRANSMISSION_DELAY);

            }

            // Assume reciever is now in sync clock mode
        }

        void calculate_trip_time() {

            std::vector<RoundTrip> times;
            int no_failure = 0;

            while (times.size() < NUMBER_SYNC_CLOCK_MESSAGES) {
                
                auto start_time = get_current_time();

                protocol.send_message(start_time);

                app::timestamp response = wait_for_response();
                app::timestamp end_time = get_current_time();

                if (!response || response != start_time) {
                    no_failure++;
                    continue;
                }

                times.push_back(RoundTrip(start_time, end_time))

                delay(TRANSMISSION_DELAY);
            }

            if (no_failure == NUMBER_SYNC_CLOCK_MESSAGES) {
                throw std::exception('Unable to sync clocks!');
            }

            return calculate_average_trip_time(times);
        }

        app::timestamp wait_for_response() {
                
            auto timeout = get_current_time() + RESPONSE_TIMEOUT_MICROSECONDS;

            _receiver_buffer.clear();

            while ( get_current_time() < timeout ) {

                if (_receiver_buffer.size()) {
                    return _receiver_buffer.pop();
                }
            }

            return NULL;

        }

        void send_clock_sync_messages(app::timestamp average_trip_time) {

            for (int i = 0; i < NUMBER_SYNC_CLOCK_MESSAGES; i++) {

                app::timestamp current_time = get_current_time();

                _protocol.send_message(current_time + average_trip_time);

            }

        }

        void finish_clock_sync_mode() {

            for (int i = 0; i < NUMBER_SYNC_CLOCK_MESSAGES; i++) {

                _protocol.send_message(FINISH_CLOCK_SYNC_MODE_MESSAGE);

            }

        }

        app::timestamp calculate_average_trip_time( std::vector<RoundTrip> times){

            app::timstamp sum_trip_time = 0;

            for (const auto time : times) {

                auto trip_time = time.end_time - time.start_time;

                sum_trip_time += trip_time;
            }


            auto average_trip_time = sum_trip_time / times.size();

            return average_trip_time / 2;

        }

    };
}