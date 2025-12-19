#include "Client.hpp"
#include "Connection.hpp"
#include "Epoll.hpp"
#include "Fd.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

static bool keep_running = true;
static void handle_interrupt(int) { keep_running = false; }

static bool is_connection_dead(Rc<Connection> const &connection) {
	return !connection.get().is_active();
}

static Fd create_listener(int const port) {
	Fd listener(socket(AF_INET, SOCK_STREAM, 0));
	if (listener.get() < 0)
		throw std::runtime_error("Failed to open TCP listener");

	int opt = 1;
	if (setsockopt(listener.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("Failed to set TCP listener options");

	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(listener.get(), (sockaddr *)&server_address,
			 sizeof(server_address)))
		throw std::runtime_error("Failed to bind TCP listener");

	try {
		listener.set_nonblocking();
	} catch (std::exception &e) {
		throw std::runtime_error("Failed to set TCP listener as non blocking");
	}

	if (listen(listener.get(), 5))
		throw std::runtime_error("Failed to start TCP listener");

	return listener;
}

int main(int argc, char *argv[]) try {
	if (argc < 3) {
		std::cerr << "Usage: ircserv [port] [password]" << std::endl;
		return 1;
	}

	std::string port_str(argv[1]);
	int port = stoi(port_str);
	std::string password = std::string(argv[2]);

	signal(SIGINT, handle_interrupt);

	std::vector<Rc<Connection> > connections;
	Server server;
	Epoll epoll;

	try {
		server = Server(create_listener(port), "ft_irc", password);
		std::cout << "Listening on port " << port << "." << std::endl;

		epoll.add_fd(server.get_listener().get(),
					 EPOLLIN); // recupere le file descriptor
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	struct epoll_event events[MAX_EVENTS];

	while (keep_running) {
		int nfds = epoll.wait(events, MAX_EVENTS, 100);

		for (int i = 0; i < nfds; i++) {
			int fd = events[i].data.fd;

			if (fd == server.get_listener().get()) {
				sockaddr_in addr = {};
				socklen_t addr_len = sizeof(sockaddr_in);
				Fd stream(accept(fd, (sockaddr *)&addr, &addr_len));

				if (stream.get() < 0) {
					if (errno != EWOULDBLOCK)
						std::cerr << "Accept error" << std::endl;
				} else if (addr.sin_family != AF_INET) {
					std::cerr << "Invalid address family: " << addr.sin_family
							  << std::endl;
				} else {
					try {
						stream.set_nonblocking();
						connections.push_back(
							Rc<Connection>(new Connection(addr, stream)));
						epoll.add_fd(stream.get(), EPOLLIN | EPOLLOUT);
						std::cerr
							<< "New connection: " << inet_ntoa(addr.sin_addr)
							<< std::endl;
					} catch (std::exception &e) {
						std::cerr << "Failed to add connection: " << e.what()
								  << std::endl;
					}
				}
			} else {
				for (std::vector<Rc<Connection> >::iterator it =
						 connections.begin();
					 it != connections.end(); it++) {
					Rc<Connection> &connection = *it;
					if (connection.get().get_stream().get() == fd) {
						try {
							if (events[i].events & EPOLLIN) {
								server.update_connection(connection);
							}
							if (events[i].events & EPOLLOUT) {
								connection.get().send();
							}
						} catch (std::exception &e) {
							ClientRegistered *client =
								dynamic_cast<ClientRegistered *>(
									&connection.get().get_client());
							if (client) {
								Nickname const &nickname =
									client->get_nickname();
								for (Server::ChannelsMap::iterator ch_it =
										 server.get_channels().begin();
									 ch_it != server.get_channels().end();
									 ch_it++) {
									Channel &channel = ch_it->second;
									channel.get_members().erase(nickname);
								}
								server.get_clients().erase(nickname);
							}
							epoll.remove_fd(fd);
							connection.get().close();
							std::cerr << "Error: " << e.what() << std::endl;
						}
						break;
					}
				}
			}
		}

		for (std::vector<Rc<Connection> >::iterator it = connections.begin(); it != connections.end(); it++) {
			Connection &connection = it->get();
			connection.try_keep_alive();
		}

		// clean
		connections.erase(std::remove_if(connections.begin(), connections.end(),
										 is_connection_dead),
						  connections.end());
	}
	return 0;
} catch (std::exception &e) {
	std::cerr << "Unhandled exception: " << e.what() << std::endl;
	return 1;
} catch (...) {
	std::cerr << "Unhandled non-standard exception" << std::endl;
	return 1;
}
