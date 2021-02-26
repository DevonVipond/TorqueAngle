#define none

#ifdef none

void setup() {}
void loop() {}

#endif

#ifdef programRx

  #include "lib/infra/protocols/espNow/ESPNowProtocol.h"

  infra::ESPNowReciever< PayloadType > reciever;

  void message_recieved_callback(const uint8_t * mac, const uint8_t *incomingData, int len) {

      memcpy(&myData, incomingData, sizeof(myData));

      Serial.println(myData.a);
  }

  void setup() {

      Serial.begin(115200);

      try {

          reciever.init(message_recieved_callback);

      } catch (infra::ConnectionFailed &e) {

          Serial.println(e.what())
      }
  }

  #endif

#ifdef programTx

  #include "lib/infra/protocols/espNow/ESPNowProtocol.h"

  typedef unsigned int PayloadType;
  static const unsigned int DELAY_PERIOD = 3000;

  infra::ESPNowTransmitter< PayloadType > transmitter;

  void setup() {
    Serial.begin(115200);
  }

  void loop() {
      static PayloadType counter = 0;

      transmitter.send_message( counter++ );


      Serial.println("sent message!");
      
      delay(DELAY_PERIOD);
  }

#endif

#ifdef testErrorVsDistance

  #include "test/errorRateVsRange/test.h"

  void setup() {
    test::errorRateVsRange::tx::setup();
  }
  void loop() {
    test::errorRateVsRange::tx::loop();
  }

#endif