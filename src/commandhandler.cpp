#include "commandhandler.h"
#include "log.h"

void CommandHandler::stateInit(Phase_t p) {
	if (p == Phase_t::Enter) {
		m_commandCount = 0;
		m_currentCommand = 0;
		stateGoto(&CommandHandler::stateAcceptCommands);
	}
}

void CommandHandler::stateAcceptCommands(Phase_t p) {
	if (p == Phase_t::Enter) {
		m_acceptCommands = true;
	}
	if (p == Phase_t::Leave) {
		m_acceptCommands = false;
	}
}

void CommandHandler::stateExecCommands(Phase_t p) {
	if (p == Phase_t::Enter) {
		m_currentCommand = 0;
	}
	if (p == Phase_t::Update) {
	}
}

void CommandHandler::addCommand(const Command& c) {
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
