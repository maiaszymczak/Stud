#pragma once

#include <string>

class User {
  public:
	User();
	User(std::string user_name, std::string real_name);
	User(User const &other);
	User &operator=(User const &other);
	~User();

  private:
	std::string _user_name;
	std::string _real_name;

  public:
	std::string const &get_user_name(void) const;
	std::string const &get_real_name(void) const;
};
