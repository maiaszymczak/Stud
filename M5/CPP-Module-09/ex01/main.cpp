#include "RPN.hpp"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << RED << "Error: could not open file." << RESET << std::endl;
        return 1;
    }

    RPN rpnCalculator;
    std::string expression = argv[1];
    rpnCalculator.evaluate(expression);

    return 0;
}