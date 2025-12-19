#pragma once

#include <stdexcept>
template <typename T> class Option {
  public:
	Option() : _inner(), _is_some(false) {}

	explicit Option(T const &some) : _inner(some), _is_some(true) {}

	Option(Option const &other)
		: _inner(other._inner), _is_some(other._is_some) {}

	Option &operator=(Option const &other) {
		if (this == &other)
			return *this;
		_is_some = other._is_some;
		_inner = other._inner;
		return *this;
	}

	~Option() {}

  private:
	T _inner;
	bool _is_some;

  public:
	bool is_some(void) const { return _is_some; }

	T &get_inner(void) {
		if (!_is_some)
			throw std::runtime_error("Option is none");
		return _inner;
	}

	T const &get_inner(void) const {
		if (!_is_some)
			throw std::runtime_error("Option is none");
		return _inner;
	}

	void set_none(void) { _is_some = false; }
	void set_some(T const &object) {
		_inner = object;
		_is_some = true;
	}
};
