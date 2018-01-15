#ifndef ARDUINO_RFID_ROBOT_RDIFREADER_H
#define ARDUINO_RFID_ROBOT_RDIFREADER_H

#include <MFRC522.h>  // https://github.com/miguelbalboa/rfid
#include "state_machine.h"
#include "utils.h"

typedef void (*OnCardRead)(char*);

class RFIDReader : public StateMachine<RFIDReader> {
   public:
	RFIDReader(byte chipSelectPin, byte resetPowerDownPin)
	    : StateMachine<RFIDReader>(this), mfrc522(chipSelectPin, resetPowerDownPin) {
	}

	void init() {
		stateGoto(&RFIDReader::stateInit);
	}

	void setOnCardRead(OnCardRead f) {
		m_onReadCard = f;
	}

	void writeNextCard(const char* data);

   private:
	bool readFromCard(byte* buffer, byte size, byte blockAddr);
	bool writeToCard(byte* buffer, byte size, byte blockAddr);

	void stateInit(Phase_t p);
	void stateWaitingForCard(Phase_t p);
	void stateReadCard(Phase_t p);
	void stateRestart(Phase_t p);

	MFRC522 mfrc522;
	MFRC522::MIFARE_Key key;
	utils::TimeOut timeout;
	OnCardRead m_onReadCard = NULL;
	char m_data2Write[32];
	bool m_writeNextCard = false;
};

#endif  // ARDUINO_RFID_ROBOT_RDIFREADER_H
