#include "Client.hpp"
#include "Nickname.hpp"

Client::~Client() {}

ClientUnauthenticated::ClientUnauthenticated() {}

ClientUnauthenticated::ClientUnauthenticated(
	ClientUnauthenticated const &other) {
	(void)other;
}

ClientUnauthenticated &
ClientUnauthenticated::operator=(ClientUnauthenticated const &other) {
	if (this == &other)
		return *this;
	return *this;
}

ClientUnauthenticated::~ClientUnauthenticated() {}

Client *ClientUnauthenticated::clone(void) const {
	return new ClientUnauthenticated(*this);
}

ClientUnregistered::ClientUnregistered() : _nickname(), _user() {}

ClientUnregistered::ClientUnregistered(ClientUnregistered const &other) {
	(void)other;
}

ClientUnregistered &
ClientUnregistered::operator=(ClientUnregistered const &other) {
	if (this == &other)
		return *this;
	return *this;
}

ClientUnregistered::~ClientUnregistered() {}

Option<Nickname> &ClientUnregistered::get_nickname(void) { return _nickname; }
Option<Nickname> const &ClientUnregistered::get_nickname(void) const {
	return _nickname;
}
Option<User> &ClientUnregistered::get_user(void) { return _user; }
Option<User> const &ClientUnregistered::get_user(void) const { return _user; }

Client *ClientUnregistered::clone(void) const {
	return new ClientUnregistered(*this);
}

ClientRegistered::ClientRegistered() : _nickname() {}

ClientRegistered::ClientRegistered(Nickname const &nickname)
	: _nickname(nickname) {}

ClientRegistered::ClientRegistered(ClientRegistered const &other) {
	(void)other;
}

ClientRegistered &ClientRegistered::operator=(ClientRegistered const &other) {
	if (this == &other)
		return *this;
	return *this;
}

ClientRegistered::~ClientRegistered() {}

Nickname &ClientRegistered::get_nickname() { return _nickname; }
Nickname const &ClientRegistered::get_nickname() const { return _nickname; }

Client *ClientRegistered::clone(void) const {
	return new ClientRegistered(*this);
}
