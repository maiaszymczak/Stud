#ifndef ITER_HPP
#define ITER_HPP

#include <cstddef>

template <typename T, typename A>
void iter(T *array, unsigned int size, void (*f)(A &arg)){
	for(unsigned int i = 0; i < size; i++)
		f(array[i]);
}

#endif
