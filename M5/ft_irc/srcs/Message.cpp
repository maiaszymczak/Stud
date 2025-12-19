#include "Message.hpp"
#include "Command.hpp"
#include <sstream>
#include <stdexcept>
#include <string>

Message::Message() : _prefix(), _command(), _params() {}

Message::Message(std::string msg) : _prefix(), _command(), _params() {
	if (msg.empty())
		throw std::runtime_error("Empty message");
	if (msg.size() < 2 || msg.at(msg.size() - 2) != '\r' ||
		msg.at(msg.size() - 1) != '\n')
		throw std::runtime_error("Message is not terminated");
	msg.resize(msg.size() - 2);

	bool has_prefix = msg.at(0) == ':';
	if (has_prefix)
		msg.erase(msg.begin());

	std::stringstream ss(msg);

	if (has_prefix)
		std::getline(ss, _prefix, ' ');

	std::string command_string;
	std::getline(ss, command_string, ' ');
	_command = Command(command_string);

	// FIXME parameters are never parsed
	while (!ss.eof()) {
		char first;
		ss.get(first);
		if (first == ':') {
			std::string param;
			while (!ss.eof()) {
				std::getline(ss, param);
			}
			_params.push_back(param);
			break;
		} else {
			std::string param;
			std::getline(ss, param, ' ');
			if (!first || param.empty()) {
				break;
			}
			param = first + param;
			_params.push_back(param);
		}
	}
}

Message::Message(Message const &other)
	: _prefix(other._prefix), _command(other._command), _params(other._params) {
}

Message &Message::operator=(Message const &other) {
	if (this == &other)
		return *this;
	_prefix = other._prefix;
	_command = other._command;
	_params = other._params;
	return *this;
}

Message::~Message() {}

std::string const &Message::get_prefix(void) const { return _prefix; }
Command const &Message::get_command(void) const { return _command; }
std::vector<std::string> const &Message::get_params(void) const {
	return _params;
}

std::string Message::to_string(void) const {
	std::stringstream ss;
	if (!_prefix.empty())
		ss << ':' << _prefix << ' ';
	ss << _command.to_string() << ' ';
	for (std::vector<std::string>::const_iterator it = _params.begin();
		 it != _params.end(); it++)
		ss << *it << ' ';
	return ss.str();
}
