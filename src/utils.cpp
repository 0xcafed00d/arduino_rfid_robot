#include "utils.h"
#include "log.h"

namespace utils {

	void logByteArray(byte* buffer, byte bufferSize) {
		char buf[3] = {};
		static const char hex[] = "0123456789ABCDEF";

		auto lp = logr << '[';

		for (byte i = 0; i < bufferSize; i++) {
			buf[0] = hex[(buffer[i] >> 4) & 0xf];
			buf[1] = hex[buffer[i] & 0xf];
			lp << buf;
		}
		lp << ']';
	}
}
