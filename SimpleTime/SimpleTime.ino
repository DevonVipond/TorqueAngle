#define programTx

#ifdef programRx

  #include "test/errorRateVsRange/test.h"

  void setup() {
    test::errorRateVsRange::rx::setup();
  }
  void loop() {
  }

  #endif

#ifdef programTx

  #include "lib/infra/protocols/espNow/ESPNowProtocol.h"
  #include "lib/infra/server/server.h"
  #include "lib/infra/message/message.h"

  typedef unsigned int PayloadType;

  infra::Server< infra::Protocol< PayloadType>, PayloadType  > server;

  void setup() { 
    Serial.begin(115200);
  }

  void loop() {
      static PayloadType counter = 0;

      server.send_message( counter++ );

      Serial.println("sent message!");
      
      delay(3000);
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