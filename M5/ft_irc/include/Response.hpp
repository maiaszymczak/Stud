#pragma once

#include "Nickname.hpp"
#include "Option.hpp"
#include <string>
class Response {
  public:
	enum ReplyCode {
		RPLY_MESSAGE = 0,
		RPLY_WELCOME = 1,
		RPLY_UMODEIS = 221,
		RPLY_WHOISUSER = 311,
		RPLY_NOTOPIC = 331,
		RPLY_TOPIC = 332,
		RPLY_MOTD = 372,
		RPLY_MOTDSTART = 376,
		RPLY_MOTDEND = 376,
		RPLY_NOSUCHNICK = 401,
		RPLY_NOSUCHCHANNEL = 403,
		RPLY_CANNOTSENDTOCHAN = 404,
		RPLY_NORECIPIENT = 411,
		RPLY_UNKNOWNCOMMAND = 421,
		RPLY_NONICKNAMEGIVEN = 431,
		RPLY_ERRONEOUSNICKNAME = 432,
		RPLY_NICKNAMEINUSE = 433,
		RPLY_NOTONCHANNEL = 442,
		RPLY_USERONCHANNEL = 443,
		RPLY_USERNOTINCHANNEL = 441,
		RPLY_NOTREGISTERED = 451,
		RPLY_NEEDMOREPARAMS = 461,
		RPLY_ALREADYREGISTERED = 462,
		RPLY_PASSWDMISMATCH = 463,
		RPLY_CHANNELISFULL = 471,
		RPLY_INVITEONLYCHAN = 473,
		RPLY_BADCHANNELKEY = 475,
		RPLY_CHANOPRIVSNEEDED = 482,
	};

  public:
	Response();
	Response(std::string const &message);
	Response(ReplyCode code);
	Response(ReplyCode code, std::string const &content);
	Response(Response const &other);
	Response &operator=(Response const &other);
	~Response();

  private:
	ReplyCode _code;
	std::string _content;

  public:
	ReplyCode get_code(void) const;
	std::string const &get_content(void) const;
	std::string to_string(std::string const &server_name,
						  Option<Nickname> const &nickname) const;
};
