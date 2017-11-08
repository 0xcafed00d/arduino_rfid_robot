#include "Arduino.h"
#include "RFIDReader.h"
#include "SPI.h"
#include "utils.h"

#include "log.h"

Logger logr(Serial);

const int RST_PIN = 9;
const int SS_PIN = 10;

RFIDReader rfid(SS_PIN, RST_PIN);

void printCardData(char* data) {
	Serial.println(data);
}

void setup() {
	// while(!Serial);  // Remove before Flight
	SPI.begin();

	rfid.init();
	rfid.setOnCardRead(&printCardData);
}

void loop() {
	logr.enable(bool(Serial));
	logr.enable(false);

	delay(10);
	rfid.stateUpdate();
}
