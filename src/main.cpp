#include "Arduino.h"
#include "SPI.h"
#include "cli.h"
#include "commandhandler.h"
#include "rfidreader.h"
#include "utils.h"

#include "log.h"

Logger logr(Serial);

const int RST_PIN = 9;
const int SS_PIN = 10;

RFIDReader rfid(SS_PIN, RST_PIN);
CommandHandler cmdHandler;

char inputBuffer[64];
CLI cmdLine(&Serial, inputBuffer, 5);

void onCardRead(char* data) {
	logr << data;
	Command c = parseCommand(data);
	logr << (uint16_t)c.type << c.duration;

	cmdHandler.addCommand(c);
}

void onLine(char* data) {
	logr << ">>>>>>" << data << "<<<<<<";
}

void setup() {
	// while(!Serial);  // Remove before Flight
	SPI.begin();

	rfid.init();
	rfid.setOnCardRead(&onCardRead);

	cmdHandler.init();
	cmdLine.setOnCLILine(&onLine);
}

void loop() {
	logr.enable(bool(Serial));
	// logr.enable(false);

	//	cmdLine.update();

	//	delay(10);
	//	rfid.stateUpdate();
	//	cmdHandler.stateUpdate();

	cmdLine.update();
}
