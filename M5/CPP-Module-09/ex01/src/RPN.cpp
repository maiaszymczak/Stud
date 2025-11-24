#include "RPN.hpp"

RPN::RPN() {}
RPN::~RPN() {}

void RPN::evaluate(const std::string& expression)
{
    for (size_t i = 0; i < expression.length(); ++i)
    {
        char token = expression[i];

        if (token == ' ')
            continue;

        if (isdigit(token))
        {
            stk.push(token - '0');
        }
        else
        {
            if (stk.size() < 2)
            {
                std::cerr << RED << "Error" << RESET << std::endl;
                return;
            }

            int b = stk.top(); stk.pop();
            int a = stk.top(); stk.pop();
            int result;

            switch (token)
            {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/':
                    if (b == 0)
                    {
                        std::cerr << RED << "Error: division by zero." << RESET << std::endl;
                        return;
                    }
                    result = a / b; break;
                default:
                    std::cerr << RED << "Error: invalid operator '" << token << "'." << RESET << std::endl;
                    return;
            }
            stk.push(result);
        }
    }

    if (stk.size() != 1)
    {
        std::cerr << RED << "Error: the user input has too many values." << RESET << std::endl;
        return;
    }

    std::cout << GREEN << "Result: " << stk.top() << RESET << std::endl;
}

