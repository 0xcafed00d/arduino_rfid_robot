#ifndef ARDUINO_RFID_ROBOT_COMMAND_HANDLER_H
#define ARDUINO_RFID_ROBOT_COMMAND_HANDLER_H

#include <Arduino.h>
#include "state_machine.h"
#include "utils.h"
#include "command.h"

class CommandHandler : public StateMachine<CommandHandler> {
   public:
	CommandHandler() : StateMachine<CommandHandler>(this) {
	}

	void init() {
		stateGoto(&CommandHandler::stateInit);
	}

	void addCommand(const Command& c) {
		if (m_commandCount == MAX_COMMANDS) {
			return;
		}
		switch (c.type) {
			case Command_t::play:
				stateGoto(&CommandHandler::stateExecCommands);
				break;
			case Command_t::clear:
				stateGoto(&CommandHandler::stateInit);
				break;
			case Command_t::stop:
				stateGoto(&CommandHandler::stateAcceptCommands);
				break;
			default:
				m_commandList[m_commandCount++] = c;
		}
	}

   private:
	void stateInit(Phase_t p);
	void stateAcceptCommands(Phase_t p);
	void stateExecCommands(Phase_t p);

	utils::TimeOut m_timeout;

	static const int MAX_COMMANDS = 128;
	int m_commandCount = 0;
	int m_currentCommand = 0;
	Command m_commandList[MAX_COMMANDS];
};

#endif  // ARDUINO_RFID_ROBOT_COMMAND_HANDLER_H
