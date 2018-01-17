#ifndef CLI_H
#define CLI_H

#include <Arduino.h>

typedef void (*OnCLILine)(char*);

class CLI {
   public:
	CLI(Stream* s, char* buffer, size_t bufferSize);
	void clear();
	void update();
	void setOnCLILine(OnCLILine f) {
		m_onLineFunc = f;
	}
	void setPrompt(const char* prompt);
	void bufferToUpper();
	void outputBuffer();

   private:
	uint8_t doEscape(char c);
	void doInsert(char c);
	void doEnter();
	void doBackSpace();
	void doDelete();
	void doCursorL();
	void doCursorR();
	void doHome();
	void doEnd();
	void setCursorPos();

	size_t m_cursorPos = 0;
	size_t m_bufferPos = 0;
	Stream* m_str;
	char* m_buffer;
	size_t m_bufferSize;
	OnCLILine m_onLineFunc;
	uint8_t m_escape;
	size_t m_promptLen = 0;
};

#endif /* CLI_H */
