#pragma once

#include "Nickname.hpp"
#include "Option.hpp"
#include "User.hpp"

class Client {
  public:
	virtual ~Client();
	virtual Client *clone(void) const = 0;
};

class ClientUnauthenticated : public Client {
  public:
	ClientUnauthenticated();
	ClientUnauthenticated(ClientUnauthenticated const &other);
	ClientUnauthenticated &operator=(ClientUnauthenticated const &other);
	~ClientUnauthenticated();

	Client *clone(void) const;
};

class ClientUnregistered : public Client {
  public:
	ClientUnregistered();
	ClientUnregistered(ClientUnregistered const &other);
	ClientUnregistered &operator=(ClientUnregistered const &other);
	~ClientUnregistered();

  private:
	Option<Nickname> _nickname;
	Option<User> _user;

  public:
	Option<Nickname> &get_nickname(void);
	Option<Nickname> const &get_nickname(void) const;
	Option<User> &get_user(void);
	Option<User> const &get_user(void) const;

	Client *clone(void) const;
};

class ClientRegistered : public Client {
  public:
	ClientRegistered();
	ClientRegistered(Nickname const &nickname);
	ClientRegistered(ClientRegistered const &other);
	ClientRegistered &operator=(ClientRegistered const &other);
	~ClientRegistered();

  private:
	Nickname _nickname;

  public:
	Nickname &get_nickname(void);
	Nickname const &get_nickname(void) const;

	Client *clone(void) const;
};
