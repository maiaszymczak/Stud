#include "Connection.hpp"
#include "Client.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <utils.hpp>

Connection::Connection()
	: _address(), _stream(-1), _in_buffer(), _send_buffer(),
	  _client(new ClientUnauthenticated()), _done(false),
	  _last_contact(std::time(NULL)), _last_ping() {}

Connection::Connection(sockaddr_in const &address, Fd const &stream)
	: _address(address), _stream(stream), _in_buffer(), _send_buffer(),
	  _client(new ClientUnauthenticated()), _done(false),
	  _last_contact(std::time(NULL)), _last_ping() {}

Connection::Connection(Connection const &other)
	: _address(other._address), _stream(other._stream),
	  _in_buffer(other._in_buffer), _send_buffer(other._send_buffer),
	  _client(other.get_client().clone()), _done(other._done),
	  _last_contact(other._last_contact), _last_ping(other._last_ping) {}

Connection &Connection::operator=(Connection const &other) {
	if (this == &other)
		return *this;
	_address = other._address;
	_stream = other._stream;
	_in_buffer = other._in_buffer;
	_send_buffer = other._send_buffer;
	_done = other._done;
	_last_contact = other._last_contact;
	_last_ping = other._last_ping;
	delete _client;
	if (other._client)
		_client = other._client;
	return *this;
}

Connection::~Connection() { delete _client; }

sockaddr_in const &Connection::get_address(void) const { return _address; }
bool Connection::is_active(void) const { return _stream.get() >= 0; }
Fd const &Connection::get_stream(void) const { return _stream; }

Client &Connection::get_client(void) {
	if (!_client)
		throw std::logic_error("Connection client is null");
	return *_client;
}

Client const &Connection::get_client(void) const {
	if (!_client)
		throw std::logic_error("Connection client is null");
	return *_client;
}

bool &Connection::get_done(void) { return _done; }
bool const &Connection::get_done(void) const { return _done; }

std::string Connection::show_client_kind(void) const {
	if (dynamic_cast<ClientUnauthenticated *>(_client))
		return "Unauthenticated";
	if (dynamic_cast<ClientUnregistered *>(_client))
		return "Unregistered";
	if (dynamic_cast<ClientRegistered *>(_client))
		return "Registered";
	return "Invalid";
}

void Connection::make_authenticated(void) {
	delete _client;
	_client = new ClientUnregistered();
	std::cerr << "Connection authenticated" << std::endl;
}

void Connection::make_registered(Nickname nickname) {
	delete _client;
	_client = new ClientRegistered(nickname);
	std::cerr << "Connection registered" << std::endl;
}

void Connection::got_contact(void) {
	_last_contact = std::time(NULL);
	_last_ping.set_none();
}

void Connection::try_keep_alive(void) {
	std::time_t now = std::time(NULL);
	double since_last_contact = std::difftime(now, _last_contact);
	if (since_last_contact > KEEP_ALIVE_TIME) {
		close();
	} else if (since_last_contact > PING_PERIOD && !_last_ping.is_some()) {
		write(std::string("PING :ft_irc\r\n")); // TODO server name
		std::cerr << "<- PING " << _stream.get() << std::endl;
		_last_ping.set_some(now);
	}
}

std::string const Connection::get_message(void) {
	std::string msg;
	if (_in_buffer.size() < 2)
		return msg;
	for (size_t i = 0; i < _in_buffer.size() - 1; i++) {
		if (_in_buffer.at(i) == '\r' && _in_buffer.at(i + 1) == '\n') {
			for (size_t j = 0; j < i + 2; j++)
				msg.push_back(_in_buffer.at(j));
			Connection::Buffer new_buffer;
			for (size_t j = i + 2; j < _in_buffer.size(); j++)
				new_buffer.push_back(_in_buffer.at(j));
			_in_buffer = new_buffer;
			break;
		}
	}
	return msg;
}

void Connection::read(void) {
	if (_stream.get() < 0)
		throw std::runtime_error("Failed to read on closed connection");
	std::string recv = _stream.recv(MSG_DONTWAIT);
	for (size_t i = 0; i < recv.size(); i++)
		_in_buffer.push_back(recv.at(i));
}

void Connection::send(void) {
	if (_stream.get() < 0)
		throw std::runtime_error("Failed to send on closed connection");
	if (_send_buffer.empty()) {
		if (_done) {
			close();
		}
		return;
	}
	try {
		size_t n_sent = _stream.send(_send_buffer, MSG_DONTWAIT | MSG_NOSIGNAL);
		if (n_sent == 0)
			return;
		Connection::Buffer new_buffer;
		for (size_t i = n_sent; i < _send_buffer.size(); i++)
			new_buffer.push_back(_send_buffer.at(i));
		_send_buffer = new_buffer;
	} catch (...) {
		throw std::runtime_error("Failed to send to stream");
	}
	if (_done && _send_buffer.empty()) {
		close();
	}
}

void Connection::close(void) {
	int fd = _stream.get();
	_stream.close();
	std::cerr << "Connection " << fd << " to "
			  << inet_ntoa(get_address().sin_addr) << " closed" << std::endl;
}
