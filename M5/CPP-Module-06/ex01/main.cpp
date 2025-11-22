#include "serialization.hpp"
#include "Colors.hpp"

int main()
{
    Data* original = new Data();
    
    std::cout << BRIGHT_CYAN << "Enter a message to serialize: " << RESET;
    std::getline(std::cin, original->data);

    std::cout << ORANGE << "\nOriginal Data address: " << original << RESET << std::endl;
    std::cout << ORANGE << "Original Data content: " << original->data << RESET << std::endl;

    uintptr_t raw = serialization::serialize(original);
    std::cout << PURPLE << "Serialized Data (uintptr_t): " << raw << RESET << std::endl;

    Data* deserialized = serialization::deserialize(raw);
    std::cout << BRIGHT_YELLOW << "Deserialized Data address: " << deserialized << RESET << std::endl;
    std::cout << BRIGHT_YELLOW << "Deserialized Data content: " << deserialized->data << RESET << std::endl;

    delete original;

    return 0;
}