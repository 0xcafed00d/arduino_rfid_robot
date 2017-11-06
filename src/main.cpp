#include "Arduino.h"
#include "RFIDReader.h"
#include "SPI.h"
#include "utils.h"

const int RST_PIN = 9;
const int SS_PIN = 10;

RFIDReader rfid(SS_PIN, RST_PIN);

void setup() {
	// while(!Serial);  // Remove before Flight
	SPI.begin();

	rfid.init();
}

void loop() {
	utils::UpdateLoggingState();
	delay(10);
	rfid.stateUpdate();
}
