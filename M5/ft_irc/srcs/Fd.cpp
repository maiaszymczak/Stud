#include "Fd.hpp"
#include "utils.hpp"
#include <cerrno>
#include <fcntl.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

static void close_fd(int *fd) {
	// std::cerr << "[FD] on_destructor" << std::endl;
	if (fd && *fd >= 0) {
		// std::cerr << "[FD] fd closed: " << *fd << std::endl;
		::close(*fd);
		*fd = -1;
	}
}

Fd::Fd() : Rc<int>(new int(-1), close_fd) {}

Fd::Fd(int const fd) : Rc<int>(new int(fd), close_fd) {}

Fd::Fd(Fd const &other) : Rc<int>(other) {}

Fd &Fd::operator=(Fd const &other) {
	if (this == &other)
		return *this;
	Rc<int>::operator=(other);
	return *this;
}

Fd::~Fd() {
	// std::cerr << "[FD] Destructor called." << std::endl;
}

bool Fd::is_valid(void) const { return get() >= 0; }

void Fd::set_nonblocking(void) const {
	if (fcntl(get(), F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Failed to set fd as non blocking");
}

std::string Fd::read(void) const {
	if (get() < 0)
		throw std::runtime_error("Failed to read invalid fd");
	size_t const READ_SIZE = 128;
	char buf[READ_SIZE];
	ssize_t n_read = ::read(get(), buf, READ_SIZE);
	if (n_read < 0)
		throw std::runtime_error("Failed to read from fd " + itos(get()));
	std::string out;
	for (ssize_t i = 0; i < n_read; i++)
		out.push_back(buf[i]);
	return out;
}

std::string Fd::recv(int const flags) const {
	if (get() < 0)
		throw std::runtime_error("Failed to read invalid fd");
	size_t const READ_SIZE = 128;
	char buf[READ_SIZE];
	errno = 0;
	ssize_t n_recv = ::recv(get(), buf, READ_SIZE, flags);
	if (n_recv < 0) {
		if (flags & MSG_DONTWAIT && errno == EWOULDBLOCK)
			return "";
		throw std::runtime_error("Failed to read from fd " + itos(get()));
	}
	std::string out;
	for (ssize_t i = 0; i < n_recv; i++)
		out.push_back(buf[i]);
	return out;
}

size_t Fd::write(std::string const &string) const {
	if (get() < 0)
		throw std::runtime_error("Failed to write to invalid fd");
	ssize_t n_written = ::write(get(), string.c_str(), string.size());
	if (n_written < 0)
		throw std::runtime_error("Failed to write to fd");
	return n_written;
}

size_t Fd::send(std::string const &string, int const flags) const {
	if (get() < 0)
		throw std::runtime_error("Failed to write to invalid fd");
	errno = 0;
	ssize_t n_sent = ::send(get(), string.c_str(), string.size(), flags);
	if (n_sent < 0) {
		if (flags & MSG_DONTWAIT && errno == EWOULDBLOCK)
			return 0;
		throw std::runtime_error("Failed to write to fd");
	}
	return n_sent;
}

void Fd::close(void) {
	// std::cerr << "[FD] Closing file descriptor." << std::endl;
	close_fd(&get());
}
