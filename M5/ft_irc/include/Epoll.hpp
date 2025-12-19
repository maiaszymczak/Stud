#pragma once
#pragma once

#include "Fd.hpp"
#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EVENTS 64

class Epoll {
  public:
	Epoll();
	Epoll(const Epoll &other);
	Epoll &operator=(const Epoll &other);
	~Epoll();

	Fd const &get_fd() const;

	void add_fd(int fd, unsigned int events);
	void modify_fd(int fd, unsigned int events);
	void remove_fd(int fd);
	int wait(struct epoll_event *events, int max_events, int timeout);

  private:
	Fd _epoll_fd;
};
