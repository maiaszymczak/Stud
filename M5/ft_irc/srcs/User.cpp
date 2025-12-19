#include "User.hpp"

User::User() : _user_name(), _real_name() {}

User::User(std::string user_name, std::string real_name)
	: _user_name(user_name), _real_name(real_name) {}

User::User(User const &other)
	: _user_name(other._user_name), _real_name(other._real_name) {}

User &User::operator=(User const &other) {
	if (this == &other)
		return *this;
	_user_name = other._user_name;
	_real_name = other._real_name;
	return *this;
}

User::~User() {}

std::string const &User::get_user_name(void) const { return _user_name; }
std::string const &User::get_real_name(void) const { return _real_name; }
