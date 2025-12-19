#pragma once

#include "Rc.hpp"
#include <string>

class Fd : public Rc<int> {
  public:
	Fd();
	Fd(int const fd);
	Fd(Fd const &other);
	Fd &operator=(Fd const &other);
	~Fd();

  public:
	bool is_valid(void) const;
	void set_nonblocking(void) const;
	std::string read(void) const;
	std::string recv(int const flags) const;
	size_t write(std::string const &string) const;
	size_t send(std::string const &string, int const flags) const;
	void close(void);
};
