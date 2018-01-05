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

void CLI::bufferToUpper() {
	char* buf = m_buffer;
	while (*buf) {
		*buf = toUpperCase(*buf);
		buf++;
	}
}

void CLI::update() {
	if (m_str->available()) {
		char c = m_str->read();

		// m_str->println((int)c, 16);
		// return;

		if (c == 0x0d) {
			doEnter();
		}

		switch (m_escape) {
			case 0:
				if (c >= ' ' && c < 127) {
					doInsert(c);
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
				m_escape = doEscape(c);
				break;
			case 3:
				m_escape = 0;  // absorb 0x7e escape terminiator
				break;
		}

		if (c == 8) {  // backspace
			doBackSpace();
		}

		m_str->print('\r');
		m_str->print(m_buffer);
		m_str->print(' ');
		setCursorPos();
	}
}

void CLI::setCursorPos() {
	m_str->print('\r');
	if (m_cursorPos) {
		m_str->print("\x1b[");
		m_str->print(m_cursorPos);
		m_str->print('C');
	}
}

uint8_t CLI::doEscape(char c) {
	switch (c) {
		case 'D':  // cursor left
			doCursorL();
			break;
		case 'C':  // cursor right
			doCursorR();
			break;
		case '3':
			doDelete();
			return 3;
			break;
		case '1':
			doHome();
			return 3;
			break;
		case '4':
			doEnd();
			return 3;
			break;
	}
	return 0;
}

void CLI::doInsert(char c) {
	if (m_bufferPos < (m_bufferSize - 1)) {
		memmove(m_buffer + m_cursorPos + 1, m_buffer + m_cursorPos, m_bufferPos - m_cursorPos);
		m_buffer[m_cursorPos++] = c;
		m_buffer[++m_bufferPos] = 0;
	}
}

void CLI::doHome() {
	m_cursorPos = 0;
}

void CLI::doEnd() {
	m_cursorPos = m_bufferPos;
}

void CLI::doEnter() {
	if (m_onLineFunc) {
		m_onLineFunc(m_buffer);
	}

	clear();
}

void CLI::doBackSpace() {
	if (m_bufferPos > 0 && m_cursorPos > 0) {
		memmove(m_buffer + m_cursorPos - 1, m_buffer + m_cursorPos, m_bufferPos - m_cursorPos);
		m_buffer[--m_bufferPos] = 0;
		m_cursorPos--;
	}
}

void CLI::doDelete() {
	if (m_cursorPos < m_bufferPos) {
		memmove(m_buffer + m_cursorPos, m_buffer + m_cursorPos + 1, m_bufferPos - m_cursorPos - 1);
		m_buffer[--m_bufferPos] = 0;
	}
}

void CLI::doCursorL() {
	if (m_cursorPos > 0)
		m_cursorPos--;
}

void CLI::doCursorR() {
	if (m_cursorPos < m_bufferPos)
		m_cursorPos++;
}
