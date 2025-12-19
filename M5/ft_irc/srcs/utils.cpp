#include <netinet/in.h>
#include <sstream>
#include <string>

int stoi(std::string const &s) {
	int i;
	std::stringstream(s) >> i;
	return i;
}

std::string itos(int const i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}
