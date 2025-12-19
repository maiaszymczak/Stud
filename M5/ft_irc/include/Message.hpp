#pragma once

#include "Command.hpp"
#include <string>
#include <vector>

class Message {
  public:
	Message();
	Message(std::string msg);
	Message(Message const &other);
	Message &operator=(Message const &other);
	~Message();

  private:
	std::string _prefix;
	Command _command;
	std::vector<std::string> _params;

  public:
	std::string const &get_prefix(void) const;
	Command const &get_command(void) const;
	std::vector<std::string> const &get_params(void) const;

	std::string to_string(void) const;
};
