#include "serialization.hpp"

serialization::serialization()
{
    std::cout << BRIGHT_BLUE << "serialization Default Constructor called" << RESET << std::endl;

}
serialization::serialization(const serialization& other)
{
    std::cout << BRIGHT_MAGENTA << "serialization Copy Constructor called" << RESET << std::endl;
    *this = other;
}

serialization::~serialization()
{
    std::cout << BRIGHT_RED << "serialization Destructor called " <<  RESET << std::endl;
}

serialization& serialization::operator=(const serialization& other)
{
    std::cout << BRIGHT_CYAN << "serialization Copy Assignment Operator called" << RESET << std::endl;
    if (this != &other)
    {
    }
    return *this;
}

uintptr_t serialization::serialize(Data* ptr)
{
    return reinterpret_cast<uintptr_t>(ptr);
}

Data* serialization::deserialize(uintptr_t raw)
{
    return reinterpret_cast<Data*>(raw);
}
