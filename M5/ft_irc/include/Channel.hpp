#pragma once

#include "Nickname.hpp"
#include <set>
#include <string>

class ChannelId {
  public:
	ChannelId();
	explicit ChannelId(const std::string &name);
	ChannelId(const ChannelId &other);
	ChannelId &operator=(const ChannelId &other);
	~ChannelId();

	std::string const &get_name() const;

	bool operator==(const ChannelId &other) const;
	bool operator<(const ChannelId &other) const;

  private:
	std::string _name;
};

class Channel {
  public:
	typedef std::set<Nickname> MembersSet;

	Channel();
	Channel(Channel const &other);
	Channel &operator=(Channel const &other);
	~Channel();

  private:
	std::string _topic;
	MembersSet _members;
	MembersSet _operators;
	MembersSet _invited;

	bool _invite_only;
	bool _topic_restricted;
	std::string _key;
	size_t _members_limit;

  public:
	std::string &get_topic(void);
	std::string const &get_topic(void) const;
	MembersSet &get_members(void);
	MembersSet const &get_members(void) const;

	bool &get_invite_only(void);
	bool const &get_invite_only(void) const;
	bool &get_topic_restricted(void);
	bool const &get_topic_restricted(void) const;
	std::string &get_key(void);
	std::string const &get_key(void) const;
	size_t &get_members_limit(void);
	size_t const &get_members_limit(void) const;

	void add_member(Nickname const &nickname, std::string const &key);
	void remove_member(Nickname const &nickname);
	bool is_member(Nickname const &nickname) const;
	void add_operator(Nickname const &nickname);
	void remove_operator(Nickname const &nickname);
	bool is_operator(Nickname const &nickname) const;
	void add_invited(Nickname const &nickname);
	void remove_invited(Nickname const &nickname);
	bool is_invited(Nickname const &nickname) const;

	class InviteOnlyException : public std::exception {
		const char *what(void) const throw();
	};

	class IsFullException : public std::exception {
		const char *what(void) const throw();
	};

	class BadKeyException : public std::exception {
		const char *what(void) const throw();
	};
};
