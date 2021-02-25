#define programTx

#ifdef programRx

  //#include "lib/infra/rx.h"
  #include "test/errorRateVsRange/test.h"

  void setup() {
    test::errorRateVsRange::rx::setup();
  }
  void loop() {
  }

  #endif

#ifdef programTx

  //#include "lib/infra/tx.h"
  #include "test/errorRateVsRange/test.h"

  void setup() {
    test::errorRateVsRange::tx::setup();
  }
  void loop() {
    test::errorRateVsRange::tx::loop();
  }

#endif