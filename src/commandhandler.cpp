#include "commandhandler.h"
#include "log.h"

void CommandHandler::stateInit(Phase_t p) {
	if (p == Phase_t::Enter) {
		logr << F("CommandHandler::stateInit");
		m_commandCount = 0;
		m_currentCommand = 0;
		stateGoto(&CommandHandler::stateAcceptCommands);
	}
}

void CommandHandler::stateAcceptCommands(Phase_t p) {
	if (p == Phase_t::Enter) {
		logr << F("CommandHandler::stateAcceptCommands");
		m_acceptCommands = true;
	}
	if (p == Phase_t::Leave) {
		m_acceptCommands = false;
	}
}

void CommandHandler::stateExecCommands(Phase_t p) {
	if (p == Phase_t::Enter) {
		logr << F("CommandHandler::stateExecCommands");
		m_currentCommand = -1;
		m_commandTime = utils::TimeOut(0);
	}
	if (p == Phase_t::Update) {
		if (m_commandTime.hasTimedOut()) {
			m_currentCommand++;
			if (m_currentCommand == m_commandCount) {
				stateGoto(&CommandHandler::stateAcceptCommands);
				return;
			}
			execCommand(m_commandList[m_currentCommand]);
			m_commandTime = utils::TimeOut(m_commandList[m_currentCommand].duration * 100);
		}
	}
	if (p == Phase_t::Leave) {
	}
}

void CommandHandler::execCommand(const Command& c) {
	logr << F("Exec Cmd #") << m_currentCommand << ':' << CommandPrint(c);
}

void CommandHandler::addCommand(const Command& c) {
	logr << F("Cmd #") << m_commandCount << ':' << CommandPrint(c) << F("Added");

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
