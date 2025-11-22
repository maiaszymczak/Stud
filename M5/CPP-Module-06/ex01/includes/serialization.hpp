#ifndef SERIALIZATION_HPP
#define SERIALIZATION_HPP

#include <iostream>
#include <stdint.h>
#include "Colors.hpp"

struct Data
{
	std::string data;
};

class serialization
{
    private:
        serialization();
        serialization(const serialization& other);
        serialization& operator=(const serialization& other);
        ~serialization();
    public:
    static uintptr_t serialize(Data* ptr);
    static Data* deserialize(uintptr_t raw);
};

#endif
