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
		logr << F("Invalid Command") << data;
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
	cmdLine.bufferToUpper();
	if (strcmp(data, "EXIT") == 0) {
		cmdLine.setPrompt("CMD> ");
		rfid.init();
		cmdLine.setOnCLILine(onCmdLine);
		return;
	}

	rfid.writeNextCard(data);
	Serial.println(F("Tap Card to Write...."));
}

void onCmdLine(char* data) {
	cmdLine.bufferToUpper();
	logr << F("CMDLine input") << data;

	if (strcmp(data, "TRON") == 0) {
		logr.enable(true);
		logr << F("Trace on");
		return;
	}

	if (strcmp(data, "TROFF") == 0) {
		logr << F("Trace off");
		logr.enable(false);
		return;
	}

	if (strcmp(data, "WRITE") == 0) {
		cmdLine.setPrompt("WRITE> ");
		cmdLine.setOnCLILine(onCmdLineWrite);
		return;
	}

	addCommand(data);
}

int ACIVITY_LED = 17;

void setup() {
	while (!Serial) {
	}  // Remove before Flight

	pinMode(ACIVITY_LED, OUTPUT);

	Mouse.begin();  // temp fix - enabling mouse stops leonardo being
	                // put unto suspend mode when plugged into linux system.
	logr.enable(false);

	SPI.begin();

	rfid.init();
	rfid.setOnCardRead(&onCardRead);

	cmdHandler.init();
	cmdLine.setOnCLILine(&onCmdLine);
	cmdLine.setPrompt("CMD> ");
}

void loop() {
	//	logr.enable(bool(Serial));

	cmdLine.update();
	rfid.stateUpdate();
	cmdHandler.stateUpdate();
}
