#include "commandhandler.h"

void CommandHandler::stateInit(Phase_t p) {
	if (p == Phase_t::Enter) {
		m_commandCount = 0;
		m_currentCommand = 0;
		stateGoto(&CommandHandler::stateAcceptCommands);
	}
}

void CommandHandler::stateAcceptCommands(Phase_t p) {
}

void CommandHandler::stateExecCommands(Phase_t p) {
}
