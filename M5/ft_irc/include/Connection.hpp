#pragma once

#include "Client.hpp"
#include "Fd.hpp"
#include <netinet/in.h>
#include <stdexcept>
#include <typeinfo>
#include <ctime>

#define KEEP_ALIVE_TIME 20
#define PING_PERIOD 10

class Connection {
  public:
	typedef std::string Buffer;
	Connection();
	Connection(sockaddr_in const &address, Fd const &stream);
	Connection(Connection const &other);
	Connection &operator=(Connection const &other);
	~Connection();

  private:
	sockaddr_in _address;
	Fd _stream;
	Buffer _in_buffer;
	Buffer _send_buffer;
	Client *_client;
	bool _done;
	std::time_t _last_contact;
	Option<std::time_t> _last_ping;

  public:
	sockaddr_in const &get_address(void) const;
	bool is_active(void) const;
	Fd const &get_stream(void) const;
	Client &get_client(void);
	Client const &get_client(void) const;
	bool &get_done(void);
	bool const &get_done(void) const;

	std::string show_client_kind(void) const;

	void make_authenticated(void);
	void make_registered(Nickname nickname);

	void got_contact(void);
	void try_keep_alive(void);

	std::string const get_message(void);

	void read(void);

	template <typename Container> void write(Container const &data) {
		if (typeid(typename Container::value_type) != typeid(char))
			throw std::runtime_error("Invalid value type");
		_send_buffer.insert(_send_buffer.end(), data.begin(), data.end());
	}

	void send(void);
	void close(void);
};
