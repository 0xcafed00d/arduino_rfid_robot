#ifndef ARDUINO_RFID_ROBOT_COMMAND_H
#define ARDUINO_RFID_ROBOT_COMMAND_H

#include <Arduino.h>

enum class Command_t : uint8_t {
	invalid = 0,
	stop,
	run,
	clear,
	forward,
	backward,
	turn_l,
	turn_r,
	beep
};

struct Command {
	Command_t type;
	uint8_t duration;  // 10th second increments. 0 = instantaneous, 255 = forever
};

struct CommandPrint : public Printable {
	CommandPrint(const Command& c) : cmd(c) {
	}
	size_t printTo(Print& p) const;

	const Command& cmd;
};

Command parseCommand(const char* s);

// return false if command processind is to stop immediately
bool execCommand(const Command_t c);

#endif /* COMMAND_H */
