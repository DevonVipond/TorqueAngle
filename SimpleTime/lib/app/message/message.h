
#ifndef LIBESPNOW_H 
#define LIBESPNOW_H

namespace message {
    template<typename T>
    class Message {
        public:

        struct message {
            T payload;
        };

        Message(T payload): _payload(payload) { }

        size_t size() {
            return sizeof(T);
        }
    };
};


#endif