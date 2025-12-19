#include "Nickname.hpp"
#include <stdexcept>

Nickname::Nickname() : _value() {}

Nickname::Nickname(const std::string &nickname) : _value(nickname) {
	if (nickname.length() > 9) {
		throw std::invalid_argument("Nickname is too long: " + nickname);
	}
}

Nickname::Nickname(Nickname const &other) : _value(other._value) {}

Nickname &Nickname::operator=(Nickname const &other) {
	if (this == &other)
		return *this;
	_value = other._value;
	return *this;
}

Nickname::~Nickname() {}

const std::string &Nickname::get_value() const { return _value; }

bool Nickname::operator<(const Nickname &other) const {
	return _value < other._value;
}

bool Nickname::operator==(const Nickname &other) const {
	return _value == other._value;
}
