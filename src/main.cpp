#include "Arduino.h"
#include "RFIDReader.h"
#include "SPI.h"
#include "utils.h"

#include "log.h"

Logger loggr(Serial);

const int RST_PIN = 9;
const int SS_PIN = 10;

RFIDReader rfid(SS_PIN, RST_PIN);

void setup() {
	// while(!Serial);  // Remove before Flight
	SPI.begin();

	rfid.init();

	loggr << "hello" << 12726 << "world";
}

void loop() {
	utils::UpdateLoggingState();
	delay(10);
	rfid.stateUpdate();
}
