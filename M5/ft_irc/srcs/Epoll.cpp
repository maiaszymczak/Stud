#include "Epoll.hpp"
#include <cerrno>
#include <cstring>
#include <stdexcept>

Epoll::Epoll() : _epoll_fd(epoll_create1(0)) {
	if (!_epoll_fd.is_valid()) {
		throw std::runtime_error("Failed to create epoll instance");
	}
}

Epoll::Epoll(const Epoll &other) : _epoll_fd(other._epoll_fd) {}

Epoll &Epoll::operator=(const Epoll &other) {
	if (this == &other)
		return *this;
	_epoll_fd = other._epoll_fd;
	return *this;
}

Epoll::~Epoll() {}

Fd const &Epoll::get_fd() const { return _epoll_fd; }

void Epoll::add_fd(int fd, unsigned int events) {
	struct epoll_event event;
	std::memset(&event, 0, sizeof(event));
	event.events = events;
	event.data.fd = fd;

	if (epoll_ctl(_epoll_fd.get(), EPOLL_CTL_ADD, fd, &event) == -1) {
		throw std::runtime_error("Failed to add fd to epoll");
	}
}

void Epoll::modify_fd(int fd, unsigned int events) {
	struct epoll_event event;
	std::memset(&event, 0, sizeof(event));
	event.events = events;
	event.data.fd = fd;

	if (epoll_ctl(_epoll_fd.get(), EPOLL_CTL_MOD, fd, &event) == -1) {
		throw std::runtime_error("Failed to modify fd in epoll");
	}
}

void Epoll::remove_fd(int fd) {
	if (epoll_ctl(_epoll_fd.get(), EPOLL_CTL_DEL, fd, NULL) == -1) {
		throw std::runtime_error("Failed to remove fd from epoll");
	}
}

int Epoll::wait(struct epoll_event *events, int max_events, int timeout) {
	errno = 0;
	int nfds = epoll_wait(_epoll_fd.get(), events, max_events, timeout);
	if (nfds == -1) {
		if (errno == EINTR)
			return 0;
		throw std::runtime_error("epoll_wait failed");
	}
	return nfds;
}
