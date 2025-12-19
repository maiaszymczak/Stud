#include "Command.hpp"
#include <stdexcept>

static size_t const N_COMMANDS = 17;
static std::string const strings[N_COMMANDS] = {
	"UNDEFINED", "PASS",  "NICK",	 "USER",  "JOIN",  "PART",
	"MODE",		 "WHOIS", "PRIVMSG", "PING",  "PONG",  "QUIT",
	"CAP",		 "KICK",  "INVITE",	 "TOPIC", "ERROR",
};

Command::Command() : _kind(COM_UNDEFINED) {}

Command::Command(std::string const &command) {
	_kind = COM_UNDEFINED;
	for (size_t i = 0; i < N_COMMANDS; i++) {
		if (command == strings[i]) {
			_kind = (CommandKind)i;
			break;
		}
	}
	if (_kind == COM_UNDEFINED)
		throw std::runtime_error("Invalid command: " + command);
}

Command::Command(Command const &other) : _kind(other._kind) {}

Command &Command::operator=(Command const &other) {
	if (this == &other)
		return *this;
	_kind = other._kind;
	return *this;
}

Command::~Command() {}

CommandKind const &Command::get_kind() const { return _kind; }

std::string Command::to_string() const { return strings[_kind]; }
