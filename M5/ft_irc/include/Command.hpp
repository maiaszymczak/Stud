#pragma once

#include <string>

enum CommandKind {
	COM_UNDEFINED,
	COM_PASS,
	COM_NICK,
	COM_USER,
	COM_JOIN,
	COM_PART,
	COM_MODE,
	COM_WHOIS,
	COM_PRIVMSG,
	COM_PING,
	COM_PONG,
	COM_QUIT,
	COM_CAP,
	COM_KICK,
	COM_INVITE,
	COM_TOPIC,
	COM_ERROR,
};

class Command {
  public:
	Command();
	Command(std::string const &command);
	Command(Command const &other);
	Command &operator=(Command const &other);
	~Command();

  private:
	CommandKind _kind;

  public:
	CommandKind const &get_kind() const;
	std::string to_string(void) const;
};
