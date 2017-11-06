
#include "RFIDReader.h"

byte calcTrailerBlock(byte blockAddress) {
	return ((blockAddress + 4) & ~0x3) - 1;
}

void RFIDReader::stateInit(Phase_t p) {
	if (p == Enter) {
		utils::Logln(F("RFID Reader enter INIT"));
		mfrc522.PCD_Init();

		// Prepare the key (used both as key A and as key B)
		// using FFFFFFFFFFFFh which is the default at chip delivery from the factory
		for (byte i = 0; i < 6; i++) {
			key.keyByte[i] = 0xFF;
		}
		stateGoto(&RFIDReader::stateWaitingForCard);
	}
}

void RFIDReader::stateWaitingForCard(Phase_t p) {
	if (p == Enter) {
		utils::Logln();
		utils::Logln();
		utils::Logln(F("RFID Reader enter WaitingForCard"));
	}
	if (p == Update) {
		if (mfrc522.PICC_IsNewCardPresent()) {
			stateGoto(&RFIDReader::stateReadCard);
		}
	}
}

void RFIDReader::stateReadCard(Phase_t p) {
	if (p == Enter) {
		utils::Logln(F("RFID Reader enter ReadCard"));
		timeout = utils::TimeOut(500);
	}
	if (p == Update) {
		if (timeout.hasTimedOut()) {
			utils::Logln("timedout");
			stateGoto(&RFIDReader::stateRestart);
		}

		byte buffer[18];
		byte size = sizeof(buffer);

		// strcpy ((char*)buffer, "music:03");

		if (mfrc522.PICC_ReadCardSerial()) {
			utils::Log(F("Card UID:"));
			utils::dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
			utils::Logln("");

			// writeToCard(buffer, size, 1);

			bool ok = readFromCard(buffer, size, 1);

			if (ok) {
				utils::Logln((char*)buffer);
			}
			stateGoto(&RFIDReader::stateRestart);
		}
	}

	if (p == Leave) {
		utils::Logln(F("RFID Reader Leave ReadCard"));
		mfrc522.PICC_HaltA();
		mfrc522.PCD_StopCrypto1();
	}
}

void RFIDReader::stateRestart(Phase_t p) {
	if (p == Enter) {
		utils::Logln(F("RFID Reader enter Restart"));
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

	utils::Logln(F("Authenticating using key A..."));
	MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
	    MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK) {
		utils::Log(F("PCD_Authenticate() failed: "));
		utils::Logln(mfrc522.GetStatusCodeName(status));
		return false;
	}

	// Read data from the block
	utils::Log(F("Reading data from block "));
	utils::Log(blockAddr);
	utils::Logln(F(" ..."));
	status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
	if (status != MFRC522::STATUS_OK) {
		utils::Log(F("MIFARE_Read() failed: "));
		utils::Logln(mfrc522.GetStatusCodeName(status));
		return false;
	}

	return true;
}

bool RFIDReader::writeToCard(byte* buffer, byte size, byte blockAddr) {
	byte trailerBlock = calcTrailerBlock(blockAddr);

	// Authenticate using key B
	Serial.println(F("Authenticating again using key B..."));
	MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
	    MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("PCD_Authenticate() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return false;
	}

	// Write data to the block
	Serial.print(F("Writing data into block "));
	Serial.print(blockAddr);
	Serial.println(F(" ..."));
	status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, buffer, 16);
	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("MIFARE_Write() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return false;
	}
	return true;
}
