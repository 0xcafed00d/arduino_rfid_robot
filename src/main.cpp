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

void onCmdLine(char* data) {
	cmdLine.bufferToUpper();
	Serial.println();
	logr << F("CMDLine input") << data;
	addCommand(data);
}

void onCardRead_write(char* data) {
	Serial.println(data);

	byte buffer[18];
	byte size = sizeof(buffer);

	strcpy((char*)buffer, "CMD:TNL:4E");

	rfid.writeToCard(buffer, size, 1);
	rfid.readFromCard(buffer, size, 1);

	Serial.println(data);
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);

	Mouse.begin();  // temp fix - enabling mouse stops leonardo being
	                // put unto suspend mode when plugged into linux system.

	while (!Serial) {
	}  // Remove before Flight

	SPI.begin();

	rfid.init();
	rfid.setOnCardRead(&onCardRead_write);

	cmdHandler.init();
	cmdLine.setOnCLILine(&onCmdLine);

	addCommand("CMD:FWD:40");
	addCommand("CMD:FWD:50");
	addCommand("CMD:FWD:60");
	addCommand("CMD:FWD:70");
}

void loop() {
	logr.enable(bool(Serial));

	cmdLine.update();
	rfid.stateUpdate();
	cmdHandler.stateUpdate();
}
