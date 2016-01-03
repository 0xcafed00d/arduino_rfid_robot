#include "Arduino.h"
#include "RFIDReader.h"
#include "SPI.h"
#include "utils.h"

utils::TimeOut inactiveTimer;
const uint32_t offTime = 30000;  // 30 seconds

void setup() {
    // while(!Serial);  // Remove before Flight
    SPI.begin();

    inactiveTimer = utils::TimeOut(offTime);

    RFIDReader::Setup();
}

void loop() {
    utils::UpdateLoggingState();
    delay(10);
    RFIDReader::stateMachine.stateAction();
}
