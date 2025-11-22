#include "Span.hpp"

int main()
{
    Span sp = Span(5);

    try
    {
        sp.addNumber(6);
        sp.addNumber(3);
        sp.addNumber(17);
        sp.addNumber(9);
        sp.addNumber(11);
        //sp.addNumber(42);
    }
    catch (const std::overflow_error &e)
    {
        std::cerr << BRIGHT_RED "Error: " << e.what() << RESET << std::endl;
    }

    try
    {
        std::cout << BRIGHT_YELLOW "Shortest Span: " << RESET << sp.shortestSpan() << std::endl;
        std::cout << BRIGHT_BLUE "Longest Span: " << RESET << sp.longestSpan() << std::endl;
    }
    catch (const std::logic_error &e)
    {
        std::cerr << BRIGHT_RED "Error: " << e.what() << RESET << std::endl;
    }

    return 0;
}