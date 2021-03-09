#include "lib/receiver.h"
#include "lib/transmiter.h"

#define rx

void setup() {

  #ifdef rx
  receiver::setupReciever();
  #endif

  #ifdef tx
  transmiter::setupTransmitter();
  #endif

}
void loop() {

  #ifdef rx
  receiver::loopReciever();
  #endif

  #ifdef tx
  transmiter::loopTransmitter();
  #endif

}
