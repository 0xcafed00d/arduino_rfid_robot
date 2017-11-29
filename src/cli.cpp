#include "cli.h"

CLI::CLI(Stream* s, char* buffer, size_t bufferSize)
    : m_str(s), m_buffer(buffer), m_bufferSize(bufferSize), m_onLineFunc(0) {
	clear();
}

void CLI::clear() {
	m_bufferPos = 0;
	m_cursorPos = 0;
	m_buffer[0] = 0;
}

void CLI::update() {
	if (m_str->available()) {
		char c = m_str->read();
		if (c == 0x0d) {
			doEnter();
		}

		m_str->println((int)c, 16);
		if (c >= ' ' && c < 128) {
			doInsert(c);
		}
		m_str->println(m_buffer);
	}
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
}

void CLI::doDelete() {
}

void CLI::doCursorL() {
}

void CLI::doCursorR() {
}
