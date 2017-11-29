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

   private:
	void doInsert(char c);
	void doEnter();
	void doBackSpace();
	void doDelete();
	void doCursorL();
	void doCursorR();

	size_t m_cursorPos = 0;
	size_t m_bufferPos = 0;
	Stream* m_str;
	char* m_buffer;
	size_t m_bufferSize;
	OnCLILine m_onLineFunc;
};

#endif /* CLI_H */
