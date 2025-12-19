#pragma once

#include <cstddef>

template <typename T> class Rc {
  public:
	typedef void (*OnDestructor)(T *);

  private:
	size_t *_ref_count;
	T *_object;
	OnDestructor _on_destructor;

  public:
	T &get(void) { return *_object; }
	T const &get(void) const { return *_object; }

  public:
	Rc() : _ref_count(new size_t), _object(new T()), _on_destructor(NULL) {
		// std::cerr << "[SHARED OBJECT] Constructor 1 called." << std::endl;
		*_ref_count = 1;
	}

	Rc(T *object)
		: _ref_count(new size_t), _object(object), _on_destructor(NULL) {
		// std::cerr << "[SHARED OBJECT] Constructor 2 called." << std::endl;
		*_ref_count = 1;
	}

	Rc(T *object, OnDestructor on_destructor)
		: _ref_count(new size_t), _object(object),
		  _on_destructor(on_destructor) {
		// std::cerr << "[SHARED OBJECT] Constructor 3 called." << std::endl;
		*_ref_count = 1;
	}

	Rc(Rc const &other)
		: _ref_count(other._ref_count), _object(other._object),
		  _on_destructor(other._on_destructor) {
		// std::cerr << "[SHARED OBJECT] Copy constructor called." << std::endl;
		(*_ref_count)++;
	}

	Rc &operator=(Rc const &other) {
		// std::cerr << "[SHARED OBJECT] Copy operator called." << std::endl;
		if (this == &other)
			return *this;
		delete _ref_count;
		_ref_count = other._ref_count;
		(*_ref_count)++;
		delete _object;
		_object = other._object;
		_on_destructor = other._on_destructor;
		return *this;
	}

	~Rc() {
		// std::cerr << "[SHARED OBJECT] Destructor called." << std::endl;
		if (*_ref_count > 0)
			(*_ref_count)--;
		// std::cerr << "[SHARED OBJECT] New ref count : " << *_ref_count <<
		// std::endl;
		if (*_ref_count == 0) {
			if (_on_destructor)
				_on_destructor(_object);
			delete _object;
			delete _ref_count;
		}
	}
};
