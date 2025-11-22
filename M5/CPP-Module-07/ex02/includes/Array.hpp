#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cstddef>
#include <exception>

template <typename T>
class Array {
	private:
		T* _elements;
		unsigned int _size;

	public:
		Array() : _elements(NULL), _size(0) {}
		Array(unsigned int n) : _elements(NULL), _size(n) {
			if (n > 0)
				_elements = new T[n]();
		}
		Array(const Array& other) : _elements(NULL), _size(other._size) {
			if (_size > 0) {
				_elements = new T[_size];
				for (unsigned int i = 0; i < _size; i++)
					_elements[i] = other._elements[i];
			}
		}
		Array& operator=(const Array& other) {
			if (this != &other) {
				if (_elements)
					delete[] _elements;
				
				_size = other._size;
				_elements = NULL;
				
				if (_size > 0) {
					_elements = new T[_size];
					for (unsigned int i = 0; i < _size; i++)
						_elements[i] = other._elements[i];
				}
			}
			return *this;
		}
		~Array() {
			if (_elements)
				delete[] _elements;
		}
		T& operator[](unsigned int index) {
			if (index >= _size)
				throw std::exception();
			return _elements[index];
		}
		const T& operator[](unsigned int index) const {
			if (index >= _size)
				throw std::exception();
			return _elements[index];
		}
		unsigned int size() const {
			return _size;
		}
};

#endif