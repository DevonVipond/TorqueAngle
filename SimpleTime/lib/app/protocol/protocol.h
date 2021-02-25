
namespace protocol {
    class Server {
        public:

        Server() {}
        virtual ~Server() {}

        virtual void start() = 0;
        virtual void loop() = 0;

    };
};