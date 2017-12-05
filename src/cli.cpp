#include "cli.h"

CLI::CLI(Stream* s, char* buffer, size_t bufferSize)
    : m_str(s), m_buffer(buffer), m_bufferSize(bufferSize), m_onLineFunc(0) {
	clear();
}

void CLI::clear() {
	m_bufferPos = 0;
	m_cursorPos = 0;
	m_buffer[0] = 0;
	m_escape = 0;
}

void CLI::update() {
	if (m_str->available()) {
		bool updateOutput = false;
		char c = m_str->read();
		if (c == 0x0d) {
			doEnter();
		}

		switch (m_escape) {
			case 0:
				if (c >= ' ' && c < 128) {
					doInsert(c);
					updateOutput = true;
				} else {
					if (c == 0x1b)
						m_escape++;
				}
				break;
			case 1:
				if (c == '[')
					m_escape++;
				break;
			case 2:
				doEscape(c);
				break;
		}

		if (c == 8) {  // backspace
			doBackSpace();
		}

		m_str->print('\r');
		m_str->print(m_buffer);
		setCursorPos();
	}
}

void CLI::setCursorPos() {
	m_str->print('\r');
	m_str->print("\x1b[");
	m_str->print(m_cursorPos);
	m_str->print('C');
}

void CLI::doEscape(char c) {
	switch (c) {
		case 'D':  // cursor left
			doCursorL();
			break;
		case 'C':  // cursor right
			doCursorR();
			break;
	}
	m_escape = 0;
}

void CLI::doInsert(char c) {
	if (m_bufferPos < (m_bufferSize - 1)) {
		memmove(m_buffer + m_cursorPos + 1, m_buffer + m_cursorPos, m_bufferPos - m_cursorPos);
		m_buffer[m_cursorPos++] = c;
		m_buffer[++m_bufferPos] = 0;
	}
}

void CLI::doEnter() {
	if (m_onLineFunc) {
		m_onLineFunc(m_buffer);
	}

	clear();
}

void CLI::doBackSpace() {
	m_str->println("doBackSpace");
}

void CLI::doDelete() {
	m_str->println("doDelete");
}

void CLI::doCursorL() {
	if (m_cursorPos > 0)
		m_cursorPos--;
}

void CLI::doCursorR() {
	m_cursorPos++;
}
