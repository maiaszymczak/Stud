#include <netinet/in.h>
#include <stdexcept>
#include <string>

int stoi(std::string const &s);
std::string itos(int const i);

template <typename T, typename S> T *clone(S *source) {
	if (!source)
		throw std::runtime_error("Null pointer given to clone");
	S *derivedptr = new S(*source);
	T *baseptr = dynamic_cast<T *>(derivedptr);
	if (baseptr != NULL) {
		return baseptr;
	}
	delete derivedptr;
	throw std::runtime_error("Invalid type given to clone");
}
