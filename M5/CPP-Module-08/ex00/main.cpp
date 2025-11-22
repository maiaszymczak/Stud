#include "easyfind.hpp"
#include <vector>
#include <list>
#include <deque>

int main()
{
    // Test avec std::vector
    std::cout << BRIGHT_CYAN "=== Test with std::vector ===" RESET << std::endl;
    std::vector<int> vec;
    for (int i = 0; i < 10; ++i)
        vec.push_back(i * 10);

    try
    {
        std::vector<int>::iterator it = easyfind(vec, 50);
        std::cout <<  PURPLE "Found: " << *it << RESET << std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << BRIGHT_RED "Error: " << e.what() << RESET << std::endl;
    }

    try
    {
        std::vector<int>::iterator it = easyfind(vec, 100);
        std::cout <<  PURPLE "Found: " << *it << RESET << std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << BRIGHT_RED "Error: " << e.what() << RESET << std::endl;
    }

    // Test avec std::list
    std::cout << BRIGHT_CYAN "\n=== Test with std::list ===" RESET << std::endl;
    std::list<int> lst;
    for (int i = 0; i < 5; ++i)
        lst.push_back(i * 2);

    try
    {
        std::list<int>::iterator it = easyfind(lst, 4);
        std::cout <<  PURPLE "Found: " << *it << RESET << std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << BRIGHT_RED "Error: " << e.what() << RESET << std::endl;
    }

    try
    {
        std::list<int>::iterator it = easyfind(lst, 99);
        std::cout <<  PURPLE "Found: " << *it << RESET << std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << BRIGHT_RED "Error: " << e.what() << RESET << std::endl;
    }
    // Test avec conteneur vide
    std::cout << BRIGHT_CYAN "\n=== Test with empty container ===" RESET << std::endl;
    std::vector<int> empty;

    try
    {
        std::vector<int>::iterator it = easyfind(empty, 42);
        std::cout <<  PURPLE "Found: " << *it << RESET << std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << BRIGHT_RED "Error: " << e.what() << RESET << std::endl;
    }

    return 0;
}