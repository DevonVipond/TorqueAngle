#include "Serial"

#include "receiver/receiver.h"
#include "transmitter/transmitter.h"

void setup() {
    Serial.begin(115200);

}

void loop()
{
    transmitter::start();
    receiver::start();
}
