#pragma once

#include <string>

class Nickname {
  public:
	Nickname();
	explicit Nickname(const std::string &nickname);
	Nickname(const Nickname &other);
	Nickname &operator=(const Nickname &other);
	~Nickname();

	const std::string &get_value() const;

	bool operator<(const Nickname &other) const;
	bool operator==(const Nickname &other) const;

  private:
	std::string _value;
};
