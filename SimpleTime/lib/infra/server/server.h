#ifndef SERVER_H
#define SERVER_H

#include "../message/message.h"


namespace infra {

    template < typename Protocol, typename PayloadType >
    class Server {
        public:
        Protocol _protocol;
        Server():_protocol() {}

        void send_message(PayloadType payload){
            infra::Message<PayloadType> message(payload);
            _protocol.send_message(message);
        }

    };

};

#endif