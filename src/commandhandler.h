#ifndef ARDUINO_RFID_ROBOT_COMMAND_HANDLER_H
#define ARDUINO_RFID_ROBOT_COMMAND_HANDLER_H

#include <Arduino.h>
#include "state_machine.h"
#include "utils.h"

enum class Command_t : uint8_t { stop, play, clear, forward, backward, turn_l, turn_r };

struct Command {
	Command_t type;
	uint8_t duration;  // 10th second increments. 0 = instantanious, 255 = forever
};

class CommandHandler : public StateMachine<CommandHandler> {
   public:
	CommandHandler() : StateMachine<CommandHandler>(this) {
	}

	void init() {
		stateGoto(&CommandHandler::stateInit);
	}

	void addCommand(const Command c) {
	}

   private:
	void stateInit(Phase_t p);
	void stateWaitingForCard(Phase_t p);
	void stateReadCard(Phase_t p);
	void stateRestart(Phase_t p);

	utils::TimeOut m_timeout;

	int m_commandCount;
	int m_currentCommand;
	Command m_commandList[128];
};

#endif  // ARDUINO_RFID_ROBOT_COMMAND_HANDLER_H
