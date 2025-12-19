#include "Response.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

Response::Response() : _code(RPLY_MESSAGE), _content() {}

Response::Response(std::string const &response)
	: _code(RPLY_MESSAGE), _content(response) {}

Response::Response(Response::ReplyCode code) : _code(code), _content() {}

Response::Response(Response::ReplyCode code, std::string const &response)
	: _code(code), _content(response) {}

Response::Response(Response const &other)
	: _code(other._code), _content(other._content) {}

Response &Response::operator=(Response const &other) {
	if (this == &other)
		return *this;
	_code = other._code;
	_content = other._content;
	return *this;
}

Response::~Response() {}

Response::ReplyCode Response::get_code(void) const { return _code; }
std::string const &Response::get_content(void) const { return _content; }

std::string Response::to_string(std::string const &server_name,
								Option<Nickname> const &nickname) const {
	std::stringstream ss;
	if (_code == RPLY_MESSAGE) {
		if (!_content.empty())
			ss << _content << "\r\n";
	} else {
		ss << ":" << server_name << " ";
		ss << std::setfill('0') << std::setw(3) << _code << " ";
		if (nickname.is_some())
			ss << nickname.get_inner().get_value() << " ";
		ss << _content << "\r\n";
	}
	return ss.str();
}
