
#ifndef LIBESPNOW_H 
#define LIBESPNOW_H

namespace infra {
    template<typename T>
    class Message {
        public:
        
        T _payload;

        Message(T payload): _payload(payload) { }

        T get_payload() { return _payload; }

        size_t size() {
            return sizeof(T);
        }
    };
};


#endif