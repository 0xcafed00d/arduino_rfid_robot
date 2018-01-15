#include "Arduino.h"
#include "SPI.h"
#include "cli.h"
#include "commandhandler.h"
#include "rfidreader.h"
#include "utils.h"

#include "Mouse.h"
#include "log.h"

Logger logr(Serial);

const int RST_PIN = 9;
const int SS_PIN = 10;

RFIDReader rfid(SS_PIN, RST_PIN);
CommandHandler cmdHandler;

char inputBuffer[64];
CLI cmdLine(&Serial, inputBuffer, 64);

void addCommand(char* data) {
	Command c = parseCommand(data);
	if (c.type == Command_t::invalid) {
		logr << F("Invalid Command");
	} else {
		logr << CommandPrint(c);
		cmdHandler.addCommand(c);
	}
}

void onCardRead(char* data) {
	logr << F("RFID input") << data;
	addCommand(data);
}

void onCmdLine(char* data);

void onCmdLineWrite(char* data) {
	if (strcmp(data, "EXIT") == 0) {
		Serial.println(F("Exit Write Mode"));
		cmdLine.setOnCLILine(onCmdLine);
		return;
	}

	rfid.writeNextCard(data);
	Serial.println(F("Tap Card to Write...."));
}

void onCmdLine(char* data) {
	cmdLine.bufferToUpper();
	Serial.println();
	logr << F("CMDLine input") << data;

	if (strcmp(data, "WRITE") == 0) {
		Serial.println(F("WRITE MODE: enter data to write"));
		cmdLine.setOnCLILine(onCmdLineWrite);
		return;
	}

	addCommand(data);
}

void setup() {
	while (!Serial) {
	}  // Remove before Flight

	pinMode(LED_BUILTIN, OUTPUT);

	Mouse.begin();  // temp fix - enabling mouse stops leonardo being
	                // put unto suspend mode when plugged into linux system.

	SPI.begin();

	rfid.init();
	rfid.setOnCardRead(&onCardRead);

	cmdHandler.init();
	cmdLine.setOnCLILine(&onCmdLine);
}

void loop() {
	logr.enable(bool(Serial));

	cmdLine.update();
	rfid.stateUpdate();
	cmdHandler.stateUpdate();
}
