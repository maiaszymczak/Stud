#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Message.hpp"
#include "Nickname.hpp"
#include "Response.hpp"
#include "User.hpp"
#include "utils.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

Server::Server()
	: _listener(-1), _name(), _password(), _clients(), _channels() {}

Server::Server(Fd const &listener, std::string const &name,
			   std::string const &password)
	: _listener(listener), _name(name), _password(password), _clients(),
	  _channels() {}

Server::Server(Server const &other)
	: _listener(other._listener), _name(other._name),
	  _password(other._password), _clients(other._clients),
	  _channels(other._channels) {}

Server &Server::operator=(Server const &other) {
	if (this == &other)
		return *this;
	_listener = other._listener;
	// std::cerr << "copy listener: " << other._listener.get() << std::endl;
	_name = other._name;
	_password = other._password;
	_clients = other._clients;
	_channels = other._channels;
	return *this;
}

Server::~Server() {}

Fd const &Server::get_listener() const { return _listener; }
std::string const &Server::get_name() const { return _name; }
std::string const &Server::get_password() const { return _password; }
Server::ClientsMap &Server::get_clients() { return _clients; }
Server::ClientsMap const &Server::get_clients() const { return _clients; }
Server::ChannelsMap &Server::get_channels() { return _channels; }
Server::ChannelsMap const &Server::get_channels() const { return _channels; }

void Server::update_connection(Rc<Connection> &connection) {
	connection.get().read();
	while (true) {
		std::string input = connection.get().get_message();
		if (input.empty())
			break;
		connection.get().got_contact();
		Message message;
		try {
			message = Message(input);
		} catch (std::exception &e) {
			std::cerr << "Failed to parse message: " << e.what() << std::endl;
			continue;
		}
		Response response = handle_message(connection, message);
		std::string response_string =
			response.to_string(_name, Option<Nickname>());
		if (!response_string.empty()) {
			std::cout << "<- " << response.to_string(_name, Option<Nickname>())
					  << std::endl;
			connection.get().write(
				response.to_string(_name, Option<Nickname>()));
		} else {
			std::cout << "<- ()\r\n" << std::endl;
		}
	}
}

Response Server::handle_message(Rc<Connection> &connection,
								Message const &message) {
	ClientUnauthenticated *unauthenticated =
		dynamic_cast<ClientUnauthenticated *>(&connection.get().get_client());
	if (unauthenticated) {
		return handle_message_unauthenticated(connection.get(), message);
	}
	ClientUnregistered *unregistered =
		dynamic_cast<ClientUnregistered *>(&connection.get().get_client());
	if (unregistered) {
		Response response = handle_message_unregistered(connection.get(),
														*unregistered, message);
		if (unregistered->get_nickname().is_some() &&
			unregistered->get_user().is_some()) {
			Nickname nickname = unregistered->get_nickname().get_inner();
			User user = unregistered->get_user().get_inner();
			_clients.insert(std::pair<Nickname, ClientEntry>(
				nickname, ClientEntry(user, connection)));
			connection.get().write(
				Response(Response::RPLY_WELCOME,
						 "Welcome to the Internet Relay Network " +
							 nickname.get_value() + "!" + user.get_user_name() +
							 "@" + _name + " !")
					.to_string(_name, Option<Nickname>(nickname)));
			connection.get().make_registered(nickname);
		}
		return response;
	}
	ClientRegistered *registered =
		dynamic_cast<ClientRegistered *>(&connection.get().get_client());
	if (registered) {
		return handle_message_registered(connection.get(), *registered,
										 message);
	}
	throw std::logic_error("Invalid client type");
}

Response Server::handle_message_unauthenticated(Connection &connection,
												Message const &message) {
	std::cerr << "Handling message from unauthenticated client -> "
			  << message.to_string() << std::endl;
	std::string password;
	switch (message.get_command().get_kind()) {
	case COM_PASS:
		if (message.get_params().empty())
			return Response(Response::RPLY_NEEDMOREPARAMS,
							"PASS :Not enough parameters");
		password = message.get_params()[0];
		if (password == _password) {
			connection.make_authenticated();
			return Response();
		} else {
			return Response(Response::RPLY_PASSWDMISMATCH,
							":Password incorrect");
		}
	case COM_PING:
		return Response("PONG :" + _name);
	case COM_PONG:
		return Response();
	case COM_CAP:
		return Response("CAP * LS :");
	case COM_QUIT:
		connection.get_done() = true;
		return Response("ERROR :");
	default:
		return Response(Response::RPLY_UNKNOWNCOMMAND,
						message.get_command().to_string() +
							" :Unknown command");
	}
}

Response Server::handle_message_unregistered(Connection &connection,
											 ClientUnregistered &client,
											 Message const &message) {
	std::cerr << "Handling message from unregistered client -> "
			  << message.to_string() << std::endl;
	User new_user;
	Nickname new_nickname;
	switch (message.get_command().get_kind()) {
	case COM_PASS:
		return Response(Response::RPLY_ALREADYREGISTERED,
						":Unauthorized command (already registered)");
	case COM_NICK:
		if (message.get_params().empty())
			return Response(Response::RPLY_NONICKNAMEGIVEN,
							":No nickname given");
		try {
			new_nickname = Nickname(message.get_params()[0]);
		} catch (...) {
			return Response(Response::RPLY_ERRONEOUSNICKNAME,
							message.get_params()[0] + " :Erroneous nickname");
		}
		if (_clients.find(new_nickname) != _clients.end()) {
			return Response(Response::RPLY_NICKNAMEINUSE,
							new_nickname.get_value() +
								" :Nickname is already in use");
		}
		client.get_nickname().set_some(new_nickname);
		std::cerr << "Client nickname set" << std::endl;
		return Response();
	case COM_USER:
		if (message.get_params().size() < 4)
			return Response(Response::RPLY_NEEDMOREPARAMS,
							"USER :Not enough parameters");
		new_user = User(message.get_params().at(0), message.get_params().at(3));
		client.get_user().set_some(new_user);
		std::cerr << "Client user set" << std::endl;
		return Response();
	case COM_PING:
		return Response("PONG :" + _name);
	case COM_PONG:
		return Response();
	case COM_CAP:
		return Response("CAP * LS :");
	case COM_QUIT:
		connection.get_done() = true;
		return Response("ERROR :");
	default:
		return Response(Response::RPLY_UNKNOWNCOMMAND,
						message.get_command().to_string() +
							" :Unknown command");
	}
}

Response Server::handle_message_registered(Connection &connection,
										   ClientRegistered &client,
										   Message const &message) {
	std::cerr << "Handling message from registered client "
			  << client.get_nickname().get_value() << " -> "
			  << message.to_string() << std::endl;
	switch (message.get_command().get_kind()) {
	case COM_PASS:
	case COM_USER:
		return Response(Response::RPLY_ALREADYREGISTERED,
						":Unauthorized command (already registered)");
	case COM_NICK:
		return handle_nick(client, message);
	case COM_JOIN:
		return handle_join(client, message);
	case COM_PART:
		return handle_part(client, message);
	case COM_PRIVMSG:
		return handle_privmsg(client, message);
	case COM_KICK:
		return handle_kick(client, message);
	case COM_INVITE:
		return handle_invite(client, message);
	case COM_TOPIC:
		return handle_topic(client, message);
	case COM_MODE:
		return handle_mode(client, message);
	case COM_QUIT:
		return handle_quit(connection, client, message);
	case COM_PING:
		return Response("PONG :" + _name);
	case COM_PONG:
		return Response();
	case COM_CAP:
		return Response("CAP * LS :");
	default:
		return Response(Response::RPLY_UNKNOWNCOMMAND,
						message.get_command().to_string() +
							" :Unknown command");
	}
}

Response Server::handle_nick(ClientRegistered &client, Message const &message) {
	if (message.get_params().empty())
		return Response(Response::RPLY_NONICKNAMEGIVEN, ":No nickname given");
	Nickname new_nickname;
	try {
		new_nickname = Nickname(message.get_params()[0]);
	} catch (...) {
		return Response(Response::RPLY_ERRONEOUSNICKNAME,
						message.get_params()[0] + " :Erroneous nickname");
	}
	if (_clients.find(new_nickname) != _clients.end()) {
		return Response(Response::RPLY_NICKNAMEINUSE,
						new_nickname.get_value() +
							" :Nickname is already in use");
	}
	client.get_nickname() = new_nickname;
	return Response();
}

Response Server::handle_join(ClientRegistered const &client,
							 Message const &message) {
	if (message.get_params().empty())
		return Response(Response::RPLY_NEEDMOREPARAMS,
						"JOIN :Not enough parameters");
	ChannelId channel_id;
	std::string key;
	try {
		channel_id = ChannelId(message.get_params()[0]);
	} catch (...) {
		return Response(Response::RPLY_NEEDMOREPARAMS,
						"JOIN :Not enough parameters");
	}
	if (message.get_params().size() > 1)
		key = message.get_params()[1];

	bool is_new_channel = _channels.find(channel_id) == _channels.end();
	if (is_new_channel) {
		_channels.insert(std::pair<ChannelId, Channel>(channel_id, Channel()));
	}
	try {
		_channels[channel_id].add_member(client.get_nickname(), key);
	} catch (const Channel::InviteOnlyException &) {
		return Response(Response::RPLY_INVITEONLYCHAN,
						channel_id.get_name() + " :Cannot join channel (+i)");
	} catch (const Channel::IsFullException &) {
		return Response(Response::RPLY_CHANNELISFULL,
						channel_id.get_name() + " :Cannot join channel (+l)");
	} catch (const Channel::BadKeyException &) {
		return Response(Response::RPLY_BADCHANNELKEY,
						channel_id.get_name() + " :Cannot join channel (+k)");
	}
	if (is_new_channel) {
		_channels[channel_id].add_operator(client.get_nickname());
	}
	Response response =
		Response(":" + client.get_nickname().get_value() + "!" +
				 _clients[client.get_nickname()].first.get_user_name() + "@" +
				 _name + " JOIN " + channel_id.get_name());
	broadcast(response, Option<Nickname>(), channel_id);
	std::string const &topic = _channels[channel_id].get_topic();
	if (topic.empty())
		return Response(Response::RPLY_NOTOPIC,
						channel_id.get_name() + " :No topic is set");
	else
		return Response(Response::RPLY_TOPIC,
						channel_id.get_name() + " :" + topic);
}

Response Server::handle_part(ClientRegistered const &client,
							 Message const &message) {
	ChannelId channel_id;
	std::string part_message;

	try {
		channel_id = ChannelId(message.get_params().at(0));
	} catch (...) {
		return Response(Response::RPLY_NEEDMOREPARAMS,
						"PART :Not enough parameters");
	}
	if (message.get_params().size() > 1)
		part_message = message.get_params().at(1);
	return remove_from_channel(channel_id, part_message, client.get_nickname());
}

Response Server::handle_privmsg(ClientRegistered const &client,
								Message const &message) {
	ChannelId channel_id;
	Channel *channel;
	Response response;
	Nickname recipient;

	if (message.get_params().size() < 2)
		return Response(Response::RPLY_NEEDMOREPARAMS,
						"PRIVMSG :Not enough parameters");
	try {
		channel_id = ChannelId(message.get_params()[0]);
		channel = &_channels[channel_id];
		if (channel->get_members().find(client.get_nickname()) ==
			channel->get_members().end())
			return Response(Response::RPLY_CANNOTSENDTOCHAN,
							channel_id.get_name() + " :Cannot send to channel");
		response =
			Response(":" + client.get_nickname().get_value() + " PRIVMSG " +
					 channel_id.get_name() + " " + message.get_params()[1]);
		broadcast(response, Option<Nickname>(client.get_nickname()),
				  channel_id);
	} catch (...) {
		try {
			recipient = Nickname(message.get_params()[0]);
			response =
				Response(":" + client.get_nickname().get_value() + " PRIVMSG " +
						 recipient.get_value() + " " + message.get_params()[1]);
			_clients[recipient].second.get().write(response.to_string(
				_name, Option<Nickname>(client.get_nickname())));
			std::cout << "message sent to " << recipient.get_value()
					  << std::endl;
		} catch (...) {
			return Response(Response::RPLY_NORECIPIENT,
							":No recipient given (PRIVMSG)");
		}
	}
	return Response();
}

Response Server::handle_kick(ClientRegistered const &client,
							 Message const &message) {
	ChannelId channel_id;
	Channel *channel;
	Nickname recipient;

	if (message.get_params().size() < 2)
		return Response(Response::RPLY_NEEDMOREPARAMS,
						"KICK :Not enough parameters");
	try {
		channel_id = ChannelId(message.get_params()[0]);
	} catch (...) {
		return Response(Response::RPLY_NOSUCHCHANNEL,
						channel_id.get_name() + " :No such channel");
	}
	try {
		recipient = Nickname(message.get_params()[1]);
	} catch (...) {
		return Response(Response::RPLY_USERNOTINCHANNEL,
						recipient.get_value() + " " + channel_id.get_name() +
							" :They aren't on that channel");
	}
	try {
		channel = &_channels.at(channel_id);
	} catch (...) {
		return Response(Response::RPLY_NOSUCHCHANNEL,
						channel_id.get_name() + " :No such channel");
	}
	if (!channel->is_member(client.get_nickname()))
		return Response(Response::RPLY_NOTONCHANNEL,
						":You're not on that channel");
	if (!channel->is_operator(client.get_nickname()))
		return Response(Response::RPLY_CHANOPRIVSNEEDED,
						channel_id.get_name() +
							" :You're not channel operator");
	return remove_from_channel(channel_id, "", recipient);
}

Response Server::handle_invite(ClientRegistered const &client,
							   Message const &message) {
	Nickname recipient;
	ChannelId channel_id;
	Channel *channel;

	if (message.get_params().size() < 2)
		return Response(Response::RPLY_NEEDMOREPARAMS,
						"INVITE :Not enough parameters");
	try {
		recipient = Nickname(message.get_params()[0]);
	} catch (...) {
		return Response(Response::RPLY_NOSUCHNICK,
						recipient.get_value() + " :No such nick/channel");
	}
	try {
		channel_id = ChannelId(message.get_params()[1]);
	} catch (...) {
		return Response(Response::RPLY_NOSUCHCHANNEL,
						channel_id.get_name() + " :No such channel");
	}
	try {
		channel = &_channels.at(channel_id);
	} catch (...) {
		return Response(Response::RPLY_NOSUCHCHANNEL,
						channel_id.get_name() + " :No such channel");
	}
	if (!channel->is_member(client.get_nickname()))
		return Response(Response::RPLY_NOTONCHANNEL,
						":You're not on that channel");
	if (channel->is_member(recipient)) {
		return Response(Response::RPLY_USERONCHANNEL,
						recipient.get_value() + " " + channel_id.get_name() +
							" :is already on channel");
	}
	try {
		_clients.at(recipient).second.get().write(
			Response("INVITE " + recipient.get_value() + " " +
					 channel_id.get_name())
				.to_string(_name, Option<Nickname>(client.get_nickname())));
	} catch (...) {
		return Response(Response::RPLY_NOSUCHNICK,
						recipient.get_value() + " :No such nick/channel");
	}
	channel->add_invited(recipient);
	return Response();
}

Response Server::handle_topic(ClientRegistered const &client,
							  Message const &message) {
	ChannelId channel_id;
	Channel *channel;

	if (message.get_params().empty())
		return Response(Response::RPLY_NEEDMOREPARAMS,
						"TOPIC :Not enough parameters");
	try {
		channel_id = ChannelId(message.get_params()[0]);
	} catch (...) {
		return Response(Response::RPLY_NOSUCHCHANNEL,
						channel_id.get_name() + " :No such channel");
	}
	try {
		channel = &_channels.at(channel_id);
	} catch (...) {
		return Response(Response::RPLY_NOSUCHCHANNEL,
						channel_id.get_name() + " :No such channel");
	}

	// Check if user is in the channel
	if (!channel->is_member(client.get_nickname()))
		return Response(Response::RPLY_NOTONCHANNEL,
						channel_id.get_name() + " :You're not on that channel");

	// Setting topic
	if (message.get_params().size() > 1) {
		// Check if topic is restricted to operators
		if (channel->get_topic_restricted() &&
			!channel->is_operator(client.get_nickname()))
			return Response(Response::RPLY_CHANOPRIVSNEEDED,
							channel_id.get_name() +
								" :You're not channel operator");

		channel->get_topic() = message.get_params()[1];
		std::cerr << "set " + channel_id.get_name() +
						 " topic to: " + channel->get_topic()
				  << std::endl;

		// Broadcast topic change to all channel members
		Response response(":" + client.get_nickname().get_value() + " TOPIC " +
						  channel_id.get_name() + " :" + channel->get_topic());
		broadcast(response, Option<Nickname>(), channel_id);
		return Response();
	}
	// Getting topic
	std::string const &topic = channel->get_topic();
	if (topic.empty())
		return Response(Response::RPLY_NOTOPIC,
						channel_id.get_name() + " :No topic is set");
	else
		return Response(Response::RPLY_TOPIC,
						channel_id.get_name() + " :" + topic);
}

Response Server::handle_mode(ClientRegistered const &client,
							 Message const &message) {
	ChannelId channel_id;
	Channel *channel;
	bool mode_add;
	Nickname recipient;

	if (message.get_params().empty())
		return Response(Response::RPLY_NEEDMOREPARAMS,
						"MODE :Not enough parameters");
	try {
		channel_id = ChannelId(message.get_params()[0]);
		channel = &_channels.at(channel_id);
	} catch (...) {
		return Response();
	}
	if (!channel->is_operator(client.get_nickname()))
		return Response(Response::RPLY_CHANOPRIVSNEEDED,
						channel_id.get_name() +
							" :You're not channel operator");
	for (size_t i = 1; i < message.get_params().size(); i++) {
		switch (message.get_params()[i][0]) {
		case '+':
			mode_add = true;
			break;
		case '-':
			mode_add = false;
			break;
		default:
			return Response(Response::RPLY_NEEDMOREPARAMS,
							"MODE :Not enough parameters");
		}
		switch (message.get_params()[i][1]) {
		case 'i':
			channel->get_invite_only() = mode_add;
			break;
		case 't':
			channel->get_topic_restricted() = mode_add;
			break;
		case 'k':
			if (mode_add) {
				if (i + 1 >= message.get_params().size())
					return Response(Response::RPLY_NEEDMOREPARAMS,
									"MODE :Not enough parameters");
				i++;
				channel->get_key() = message.get_params()[i];
			} else {
				channel->get_key() = "";
			}
			break;
		case 'o':
			if (i + 1 >= message.get_params().size())
				return Response(Response::RPLY_NEEDMOREPARAMS,
								"MODE :Not enough parameters");
			i++;
			recipient = Nickname(message.get_params()[i]);
			try {
				if (mode_add) {
					channel->add_operator(recipient);
				} else {
					channel->remove_operator(recipient);
				}
			} catch (...) {
				return Response(Response::RPLY_USERNOTINCHANNEL,
								recipient.get_value() + " " +
									channel_id.get_name() +
									" :They aren't on that channel");
			}
			break;
		case 'l':
			if (mode_add) {
				if (i + 1 >= message.get_params().size())
					return Response(Response::RPLY_NEEDMOREPARAMS,
									"MODE :Not enough parameters");
				i++;
				channel->get_members_limit() = stoi(message.get_params()[i]);
			} else {
				channel->get_members_limit() = 0;
			}
			break;
		default:
			break;
		}
	}
	return Response();
}

Response Server::handle_quit(Connection &connection,
							 ClientRegistered const &client,
							 Message const &message) {
	std::string quit_message;
	if (message.get_params().size() > 0)
		quit_message = message.get_params().at(0);
	for (ChannelsMap::iterator it = _channels.begin(); it != _channels.end();
		 it++) {
		Channel &channel = it->second;
		if (channel.get_members().find(client.get_nickname()) !=
			channel.get_members().end())
			remove_from_channel(it->first, quit_message, client.get_nickname());
	}
	_clients.erase(client.get_nickname());
	connection.get_done() = true;
	return Response("ERROR :");
}

void Server::broadcast(Response const &response, Option<Nickname> const &sender,
					   ChannelId const &channel_id) {
	std::string response_string = response.to_string(_name, sender);
	if (response_string.empty()) {
		std::cerr << "Aborting broadcast of empty message" << std::endl;
		return;
	}
	Channel const &channel = _channels[channel_id];
	for (Channel::MembersSet::iterator it = channel.get_members().begin();
		 it != channel.get_members().end(); it++) {
		Nickname const &member = *it;
		if (sender.is_some() && sender.get_inner() == member) {
			continue;
		}
		std::cerr << "Sending message to client: " << member.get_value()
				  << " -> " << response_string;
		_clients[member].second.get().write(response_string);
	}
}

Response Server::remove_from_channel(ChannelId const &channel_id,
									 std::string const &message,
									 Nickname const &nickname) {
	if (_channels.find(channel_id) == _channels.end())
		return Response(Response::RPLY_NOSUCHCHANNEL,
						channel_id.get_name() + " :No such channel");
	Channel &channel = _channels[channel_id];
	if (channel.get_members().find(nickname) == channel.get_members().end())
		return Response(Response::RPLY_NOTONCHANNEL,
						":You're not on that channel");
	Response response(":" + nickname.get_value() + "!" +
					  _clients[nickname].first.get_user_name() + "@" + _name +
					  " PART " + channel_id.get_name() + " " + message);
	broadcast(response, Option<Nickname>(), channel_id);
	channel.get_members().erase(nickname);
	return Response();
}
