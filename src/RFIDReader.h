#ifndef ARDUINO_RFID_ROBOT_RDIFREADER_H
#define ARDUINO_RFID_ROBOT_RDIFREADER_H

#include <MFRC522.h>  // https://github.com/miguelbalboa/rfid
#include "state_machine.h"
#include "utils.h"

class RFIDReader : public StateMachine<RFIDReader> {
   public:
	RFIDReader(byte chipSelectPin, byte resetPowerDownPin)
	    : mfrc522(chipSelectPin, resetPowerDownPin), StateMachine<RFIDReader>(this) {
	}

	void init() {
		stateGoto(&RFIDReader::stateInit);
	}

   private:
	void stateInit(Phase_t p);
	void stateWaitingForCard(Phase_t p);
	void stateReadCard(Phase_t p);
	void stateRestart(Phase_t p);

	bool readFromCard(byte* buffer, byte size, byte blockAddr);
	bool writeToCard(byte* buffer, byte size, byte blockAddr);

	MFRC522 mfrc522;
	MFRC522::MIFARE_Key key;
	utils::TimeOut timeout;
};

#endif  // ARDUINO_RFID_ROBOT_RDIFREADER_H
