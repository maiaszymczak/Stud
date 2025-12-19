#include "Channel.hpp"
#include <stdexcept>

ChannelId::ChannelId() : _name() {}

// ChannelId implementation
ChannelId::ChannelId(const std::string &name) : _name(name) {
	if (name.length() > 50) {
		throw std::invalid_argument("Channel name is too long: " + name);
	}
	if (name.empty() || !(name[0] != '#' || name[0] != '!' || name[0] != '&' ||
						  name[0] != '+')) {
		throw std::invalid_argument(
			"Channel name must start with a '#', '!', '&' or '+': " + name);
	}
	if (name.find(' ') != std::string::npos) {
		throw std::invalid_argument("Channel name must not contain any space");
	}
}

ChannelId::ChannelId(const ChannelId &other) : _name(other._name) {}

ChannelId &ChannelId::operator=(const ChannelId &other) {
	if (this == &other)
		return *this;
	_name = other._name;
	return *this;
}

ChannelId::~ChannelId() {}

std::string const &ChannelId::get_name() const { return _name; }

bool ChannelId::operator==(const ChannelId &other) const {
	return _name == other._name;
}

bool ChannelId::operator<(const ChannelId &other) const {
	return _name < other._name;
}

// Channel implementation
Channel::Channel()
	: _topic(), _members(), _operators(), _invited(), _invite_only(false),
	  _topic_restricted(false), _key(), _members_limit(0) {}

Channel::Channel(Channel const &other)
	: _topic(other._topic), _members(other._members),
	  _operators(other._operators), _invited(other._invited),
	  _invite_only(other._invite_only),
	  _topic_restricted(other._topic_restricted), _key(other._key),
	  _members_limit(other._members_limit) {}

Channel &Channel::operator=(Channel const &other) {
	if (this == &other)
		return *this;
	_topic = other._topic;
	_members = other._members;
	_operators = other._operators;
	_invited = other._invited;
	_invite_only = other._invite_only;
	_topic_restricted = other._topic_restricted;
	_key = other._key;
	_members_limit = other._members_limit;
	return *this;
}

Channel::~Channel() {}

std::string &Channel::get_topic(void) { return _topic; }
std::string const &Channel::get_topic(void) const { return _topic; }

Channel::MembersSet &Channel::get_members(void) { return _members; }
Channel::MembersSet const &Channel::get_members(void) const { return _members; }
bool &Channel::get_invite_only(void) { return _invite_only; }
bool const &Channel::get_invite_only(void) const { return _invite_only; }
bool &Channel::get_topic_restricted(void) { return _topic_restricted; }
bool const &Channel::get_topic_restricted(void) const {
	return _topic_restricted;
}
std::string &Channel::get_key(void) { return _key; }
std::string const &Channel::get_key(void) const { return _key; }
size_t &Channel::get_members_limit(void) { return _members_limit; }
size_t const &Channel::get_members_limit(void) const { return _members_limit; }

void Channel::add_member(Nickname const &nickname, std::string const &key) {
	if (!_key.empty() && key != _key)
		throw BadKeyException();
	if (_members_limit > 0 && _members.size() >= _members_limit)
		throw IsFullException();
	if (_invite_only && !is_invited(nickname))
		throw InviteOnlyException();
	remove_invited(nickname);
	_members.insert(nickname);
}

void Channel::remove_member(Nickname const &nickname) {
	_members.erase(nickname);
}

bool Channel::is_member(Nickname const &nickname) const {
	return _members.find(nickname) != _members.end();
}

void Channel::add_operator(Nickname const &nickname) {
	if (!is_member(nickname))
		throw std::runtime_error(
			"Cannot make user that is not on channel operator");
	_operators.insert(nickname);
}

void Channel::remove_operator(Nickname const &nickname) {
	_operators.erase(nickname);
}

bool Channel::is_operator(Nickname const &nickname) const {
	return _operators.find(nickname) != _operators.end();
}

void Channel::add_invited(Nickname const &nickname) {
	if (is_member(nickname))
		return;
	_invited.insert(nickname);
}

void Channel::remove_invited(Nickname const &nickname) {
	_invited.erase(nickname);
}

bool Channel::is_invited(Nickname const &nickname) const {
	return _invited.find(nickname) != _operators.end();
}

const char *Channel::InviteOnlyException::what(void) const throw() {
	return "Invalid key";
}

const char *Channel::IsFullException::what(void) const throw() {
	return "Channel's members limit exceeded";
}

const char *Channel::BadKeyException::what(void) const throw() {
	return "Channel is invite-only";
}
