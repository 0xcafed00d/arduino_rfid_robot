
#include "rfidreader.h"
#include "log.h"

byte calcTrailerBlock(byte blockAddress) {
	return ((blockAddress + 4) & ~0x3) - 1;
}

void RFIDReader::stateInit(Phase_t p) {
	if (p == Enter) {
		logr << F("RFID Reader enter INIT");
		mfrc522.PCD_Init();

		// Prepare the key (used both as key A and as key B)
		// using FFFFFFFFFFFFh which is the default at chip delivery from the factory
		for (byte i = 0; i < 6; i++) {
			key.keyByte[i] = 0xFF;
		}
		stateGoto(&RFIDReader::stateWaitingForCard);
		m_writeNextCard = false;
	}
}

void RFIDReader::writeNextCard(const char* data) {
	m_writeNextCard = true;
	strcpy(m_data2Write, data);
}

void RFIDReader::stateWaitingForCard(Phase_t p) {
	if (p == Enter) {
		logr << F("RFID Reader enter WaitingForCard");
	}
	if (p == Update) {
		if (mfrc522.PICC_IsNewCardPresent()) {
			stateGoto(&RFIDReader::stateReadCard);
		}
	}
}

void RFIDReader::stateReadCard(Phase_t p) {
	if (p == Enter) {
		logr << F("RFID Reader enter ReadCard");
		timeout = utils::TimeOut(500);
	}
	if (p == Update) {
		if (timeout.hasTimedOut()) {
			logr << F("timedout");
			stateGoto(&RFIDReader::stateRestart);
		}

		byte buffer[18];
		byte size = sizeof(buffer);

		if (mfrc522.PICC_ReadCardSerial()) {
			logr << F("Card UID:");
			utils::logByteArray(mfrc522.uid.uidByte, mfrc522.uid.size);

			if (m_writeNextCard) {
				bool ok = writeToCard((byte*)m_data2Write, strlen(m_data2Write) + 1, 1);
			} else {
				bool ok = readFromCard(buffer, size, 1);
				if (ok) {
					logr << (char*)buffer;
					if (m_onReadCard) {
						m_onReadCard((char*)buffer);
					}
				}
			}
			stateGoto(&RFIDReader::stateRestart);
		}
	}

	if (p == Leave) {
		logr << F("RFID Reader Leave ReadCard");
		mfrc522.PICC_HaltA();
		mfrc522.PCD_StopCrypto1();
	}
}

void RFIDReader::stateRestart(Phase_t p) {
	if (p == Enter) {
		logr << F("RFID Reader enter Restart");
		timeout = utils::TimeOut(500);
	}

	if (p == Update) {
		if (timeout.hasTimedOut()) {
			stateGoto(&RFIDReader::stateWaitingForCard);
		}
	}
}

bool RFIDReader::readFromCard(byte* buffer, byte size, byte blockAddr) {
	byte trailerBlock = calcTrailerBlock(blockAddr);

	logr << F("Authenticating using key A...");
	MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
	    MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK) {
		logr << F("PCD_Authenticate() failed: ") << mfrc522.GetStatusCodeName(status);
		return false;
	}

	// Read data from the block
	logr << F("Reading data from block ") << blockAddr << F(" ...");
	status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
	if (status != MFRC522::STATUS_OK) {
		logr << F("MIFARE_Read() failed: ") << mfrc522.GetStatusCodeName(status);
		return false;
	}

	return true;
}

bool RFIDReader::writeToCard(byte* buffer, byte size, byte blockAddr) {
	byte trailerBlock = calcTrailerBlock(blockAddr);

	// Authenticate using key B
	logr << F("Authenticating again using key B...");
	MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
	    MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK) {
		logr << F("PCD_Authenticate() failed: ") << mfrc522.GetStatusCodeName(status);
		return false;
	}

	// Write data to the block
	logr << F("Writing data into block ") << blockAddr << F(" ...");
	status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, buffer, 16);
	if (status != MFRC522::STATUS_OK) {
		logr << F("MIFARE_Write() failed: ") << mfrc522.GetStatusCodeName(status);
		return false;
	}
	return true;
}
