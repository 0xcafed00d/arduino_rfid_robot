#include "command.h"
#include <Arduino.h>

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

	auto i = 0;
	while (cmdstrs[i]) {
		if (memcmp(cmdstrs + i * 3, s + 4, 3) == 0) {
			return {Command_t(i), (uint8_t)strtoul(s + 8, NULL, 16)};
		}
		i++;
	}
	return {Command_t::invalid, 0};
}

bool execCommand(const Command_t c) {
	return false;
}

size_t CommandPrint::printTo(Print& p) const {
	size_t sz = 0;
	sz += p.print(F("CMD:"));
	for (int n = 0; n < 3; n++) {
		sz += p.print(cmdstrs[int(cmd.type) * 3 + n]);
	}
	sz += p.print(F(":0x"));
	sz += p.print(cmd.duration, HEX);
	return sz;
}
