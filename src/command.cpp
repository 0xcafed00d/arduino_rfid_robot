#include <Arduino.h>
#include "command.h"

// command format:
// CMD:XXX:YY
// XXX = 3 char command code
// YY = 2 hex digit command paramater - usually time duration.

// invalid		NUL, 00
// stop			STO, 00
// play			PLA, 00
// clear		CLR, 00
// forward		FWD, Duration
// backward		BKW, Duration
// turn_l		TNL, Duration
// turn_r		TNR, Duration
// beep			BEP, Tone

static const char* cmdstrs = "NULSTOPLACLRFWDBKWTNLTNRBEP";

Command parseCommand(const char* s) {
	if (strlen(s) != 10 || memcmp(s, "CMD:", 4)) {
		return {Command_t::invalid, 0};
	}

	auto c = cmdstrs;
	auto i = 0;
	while (*c) {
		if (memcmp(c, s + 4, 3) == 0) {
			return {Command_t(i), (uint8_t)strtoul(s + 8, NULL, 16)};
		}
		c += 3;
		i++;
	}
	return {Command_t::invalid, 0};
}

bool execCommand(const Command_t c) {
	return false;
}
