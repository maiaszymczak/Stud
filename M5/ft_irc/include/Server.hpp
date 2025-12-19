#pragma once

#include "Channel.hpp"
#include "Connection.hpp"
#include "Fd.hpp"
#include "Message.hpp"
#include "Response.hpp"
#include "User.hpp"
#include <map>
#include <string>
#include <utility>

typedef std::pair<User, Rc<Connection> > ClientEntry;

class Server {
  public:
	typedef std::map<Nickname, ClientEntry> ClientsMap;
	typedef std::map<ChannelId, Channel> ChannelsMap;

	Server();
	Server(Fd const &listener, std::string const &name,
		   std::string const &password);
	Server(Server const &other);
	Server &operator=(Server const &other);
	~Server();

  private:
	Fd _listener;
	std::string _name;
	std::string _password;
	ClientsMap _clients;
	ChannelsMap _channels;

  public:
	Fd const &get_listener() const;
	std::string const &get_name() const;
	std::string const &get_password() const;
	ClientsMap &get_clients();
	ClientsMap const &get_clients() const;
	ChannelsMap &get_channels();
	ChannelsMap const &get_channels() const;

	void update_connection(Rc<Connection> &connection);
	Response handle_message(Rc<Connection> &connection, Message const &message);
	Response handle_message_unauthenticated(Connection &connection,
											Message const &message);
	Response handle_message_unregistered(Connection &connection,
										 ClientUnregistered &client,
										 Message const &message);
	Response handle_message_registered(Connection &connection,
									   ClientRegistered &client,
									   Message const &message);

	Response handle_nick(ClientRegistered &client,
						 Message const &message);
	Response handle_join(ClientRegistered const &client,
						 Message const &message);
	Response handle_part(ClientRegistered const &client,
						 Message const &message);
	Response handle_privmsg(ClientRegistered const &client,
							Message const &message);
	Response handle_kick(ClientRegistered const &client,
						 Message const &message);
	Response handle_invite(ClientRegistered const &client,
						   Message const &message);
	Response handle_topic(ClientRegistered const &client,
						  Message const &message);
	Response handle_mode(ClientRegistered const &client,
						 Message const &message);
	Response handle_quit(Connection &connection, ClientRegistered const &client,
						 Message const &message);

	void broadcast(Response const &response, Option<Nickname> const &sender,
				   ChannelId const &channel_id);
	Response remove_from_channel(ChannelId const &channel_id,
								 std::string const &message,
								 Nickname const &nickname);
};
