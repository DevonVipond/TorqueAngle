#define programRx

#ifdef programRx

  #include "lib/infra/rx.h"

  void setup() {
    rx::setup();
  }
  void loop() {
    rx::loop();
  }

  #endif

#ifdef programTx

  #include "lib/infra/tx.h"

  void setup() {
    tx::setup()
  }
  void loop() {
    tx::loop();
  }

#endif